#include "VnlcParser.hpp"
#include "../ast/declaration/VnlcDeclarationNode.hpp"
#include "../ast/declaration/VnlcExportDeclarationNode.hpp"
#include "../ast/declaration/VnlcImportDeclarationNode.hpp"
#include "../ast/module/VnlcModuleNode.hpp"
#include "../error/VnlcIllegalModuleOrPackageNameError.hpp"
#include "../error/VnlcInternalError.hpp"
#include "../error/VnlcOutOfRangeError.hpp"
#include "../error/VnlcSyntaxError.hpp"
#include "inherited/VnlcModuleParsingContext.hpp"
#include "inherited/VnlcTypeDeclarationParsingContext.hpp"
#include "inherited/VnlcVariableDeclarationParsingContext.hpp"
#include "synthesized/VnlcImportDeclarationParsingResult.hpp"
#include "synthesized/VnlcModuleParsingResult.hpp"
#include "synthesized/VnlcTopIdentifierDeclarationParsingResult.hpp"
#include "synthesized/VnlcVariableDeclarationParsingResult.hpp"
#include <memory>
#include <sstream>

VnlcParser::VnlcParser(VnlcLexer&& lexer, unsigned int maxBufferSize) : lexer(std::move(lexer)), tokenBuffer(), currentTokenIndex(0), bufferSize(0) {
    for (unsigned int i = 0; i < maxBufferSize; i++) {
        while (lexer.hasNext() && lexer.next().getType() == VnlcTokenType::BLANK) {
            // Skip blank tokens when filling the initial buffer
        }

        if (lexer.hasNext()) {
            tokenBuffer.push_back(std::move(lexer.next()));
        }
    }

    bufferSize = tokenBuffer.size();
}

bool VnlcParser::hasNextToken() const {
    return lexer.hasNext() || currentTokenIndex < bufferSize;
}

const VnlcToken& VnlcParser::peek(unsigned int offset) const {
    if (currentTokenIndex + offset < bufferSize) {
        return tokenBuffer[currentTokenIndex + offset];
    } else {
        throw VnlcOutOfRangeError("Peek offset exceeds buffer size");
    }
}

const VnlcToken& VnlcParser::peek() const {
    return peek(0);
}

void VnlcParser::fillBuffer() {
    bool blank = false;
    tokenBuffer.clear();

    for (unsigned int i = 0; i < bufferSize && lexer.hasNext(); i = blank ? i : i + 1) {
        VnlcToken token = lexer.next();
        if (token.getType() == VnlcTokenType::BLANK) {
            blank = true;
        } else {
            blank = false;
            tokenBuffer.push_back(std::move(token));
        }
    }

    bufferSize = tokenBuffer.size();
}

void VnlcParser::advance() {
    if (peek().getType() == VnlcTokenType::END_OF_FILE) {
        return;
    }

    if (currentTokenIndex < bufferSize) {
        currentTokenIndex++;
        if (currentTokenIndex == bufferSize && lexer.hasNext()) {
            fillBuffer();
            currentTokenIndex = 0;
        }
    }
}

void VnlcParser::skipNewlines() {
    while (hasNextToken() && check(VnlcTokenType::NEWLINE)) {
        advance();
    }
}

bool VnlcParser::check(VnlcTokenType expectedType) const {
    return hasNextToken() && peek().getType() == expectedType;
}

bool VnlcParser::check(std::span<VnlcTokenType> expectedTypes) const {
    for (unsigned int i = 0; i < expectedTypes.size(); i++) {
        if (!hasNextToken() || peek(i).getType() != expectedTypes[i]) {
            return false;
        }
    }
    return true;
}

bool VnlcParser::match(VnlcTokenType expectedType) {
    if (check(expectedType)) {
        advance();
        return true;
    }
    return false;
}

bool VnlcParser::matchSeparatorEndOfLine() {
    if (check(VnlcTokenType::NEWLINE)) {
        skipNewlines();
        return true;
    } else if (check(VnlcTokenType::END_OF_FILE)) {
        return true;
    }
    return false;
}

std::unique_ptr<VnlcModuleNode> VnlcParser::parse(const VnlcConfig& config) {
    VnlcModuleParsingContext context(config);
    auto result = parseModule(context);

    return std::move(result.moduleNode);
}

VnlcModuleParsingResult VnlcParser::parseModule(VnlcModuleParsingContext context) {
    std::vector<std::unique_ptr<VnlcImportDeclarationNode>> importDeclarations;
    std::vector<std::unique_ptr<VnlcDeclarationNode>> declarations;
    std::vector<std::unique_ptr<VnlcExportDeclarationNode>> exportDeclarations;
    std::string name;
    std::string fullName;

    std::string prefix = context.config.packageRootPath.filename().string();
    std::string fullPath = context.config.inputFilePath.string();

    if (!fullPath.starts_with(prefix)) {
        throw VnlcInternalError("Module file path is outside the package root");
    }
    fullPath.erase(0, prefix.length());

    if (fullPath.ends_with(".vnl")) {
        fullPath.erase(fullPath.length() - 4);
    } else {
        throw VnlcIllegalModuleOrPackageNameError("Module file name must end with .vnl");
    }

    std::stringstream ss(fullPath);
    std::string namePart;
    while (std::getline(ss, namePart, std::filesystem::path::preferred_separator)) {
        if (namePart.empty()) {
            throw VnlcIllegalModuleOrPackageNameError("Module name contains empty part");
        }

        std::stringstream namePartStream(namePart);
        VnlcLexer namePartLexer(namePartStream);
        if (!(namePartLexer.hasNext() && namePartLexer.next().getType() == VnlcTokenType::IDENTIFIER)) {
            throw VnlcIllegalModuleOrPackageNameError(namePart);
        } else if (!(namePartLexer.hasNext() && namePartLexer.next().getType() == VnlcTokenType::END_OF_FILE)) {
            throw VnlcIllegalModuleOrPackageNameError(namePart);
        }

        fullName += namePart + ".";
    }

    if (fullName.ends_with(".")) {
        fullName.pop_back();
    }
    name = fullName.substr(fullName.find_last_of('.') + 1);

    skipNewlines();

    VnlcToken firstToken = peek();

    while (check(VnlcTokenType::IMPORT)) {
        auto result = parseImportDeclaration();
        importDeclarations.push_back(std::move(result.declaration));
    }

    while (!check(VnlcTokenType::EXPORT)) {
        auto result = parseTopIdentifierDeclaration();
        declarations.push_back(std::move(result.declaration));
    }

    while (check(VnlcTokenType::EXPORT)) {
        auto result = parseExportDeclaration();
        exportDeclarations.push_back(std::move(result.declaration));
    }

    VnlcToken lastToken = peek();

    std::unique_ptr<VnlcModuleNode> node =
        std::make_unique<VnlcModuleNode>(std::move(name), std::move(fullName), std::move(importDeclarations), std::move(declarations), std::move(exportDeclarations), firstToken, lastToken);

    return VnlcModuleParsingResult{
        .moduleNode = std::move(node),
    };
}

VnlcTopIdentifierDeclarationParsingResult VnlcParser::parseTopIdentifierDeclaration() {
    bool hasMetadata = false;
    std::vector<VnlcDeclarationItem::MetadataTerm> metadataTerms;

    VnlcToken firstToken = peek();

    if (check(VnlcTokenType::METADATA)) {
        hasMetadata = true;

        auto result = parseMetadata();
        metadataTerms = std::move(result.metadata);
    }

    if (check(VnlcTokenType::VAR) || check(VnlcTokenType::LET) || check(VnlcTokenType::CONST)) {
        VnlcVariableDeclarationParsingContext context{
            .position = VnlcVariableDeclarationParsingContext::Position::TOP_LEVEL,
            .hasMetadata = hasMetadata,
            .metadataTerms = std::move(metadataTerms),
        };
        auto result = parseVariableDeclaration(context);

        VnlcToken lastToken = peek();
        result.declaration->resetPosition(firstToken, lastToken);

        return VnlcTopIdentifierDeclarationParsingResult{
            .declaration = std::move(result.declaration),
        };
    } else if (check(VnlcTokenType::FUNC) || check(VnlcTokenType::NATIVE)) {
        VnlcFunctionDeclarationParsingContext context{
            .kind = check(VnlcTokenType::FUNC) ? VnlcFunctionDeclarationType::Kind::REGULAR : VnlcFunctionDeclarationType::Kind::NATIVE,
            .context = VnlcFunctionDeclarationType::Context::TOP_LEVEL,
            .accessModifier = VnlcFunctionDeclarationType::AccessModifier::PUBLIC,
            .binding = VnlcFunctionDeclarationType::Binding::STATIC,
            .hasMetadata = hasMetadata,
            .metadataTerms = std::move(metadataTerms),
        };
        auto result = parseFunctionDeclaration(context);

        VnlcToken lastToken = peek();
        result.declaration->resetPosition(firstToken, lastToken);

        return VnlcTopIdentifierDeclarationParsingResult{
            .declaration = std::move(result.declaration),
        };
    } else if (check(VnlcTokenType::CLASS) || check(VnlcTokenType::INTERFACE) || check(VnlcTokenType::ENUM) || check(VnlcTokenType::TYPE) || check(VnlcTokenType::FINAL)) {
        VnlcTypeDeclarationParsingContext context{
            .hasMetadata = hasMetadata,
            .metadataTerms = std::move(metadataTerms),
        };
        auto result = parseTypeDeclaration(context);

        VnlcToken lastToken = peek();
        result.declaration->resetPosition(firstToken, lastToken);

        return VnlcTopIdentifierDeclarationParsingResult{
            .declaration = std::move(result.declaration),
        };
    } else {
        throw VnlcSyntaxError("Expected variable, function or type declaration", peek().getLine(), peek().getColumn());
    }
}

VnlcImportDeclarationParsingResult VnlcParser::parseImportDeclaration() {
    VnlcToken firstToken = peek();

    if (!match(VnlcTokenType::IMPORT)) {
        throw VnlcSyntaxError("Expected 'import' keyword", peek().getLine(), peek().getColumn());
    }
    skipNewlines();

    auto result = parseImportPath();

    VnlcToken lastToken = peek();

    std::unique_ptr<VnlcImportDeclarationNode> node = std::make_unique<VnlcImportDeclarationNode>(result.relative, std::move(result.paths), firstToken, lastToken);

    return VnlcImportDeclarationParsingResult{
        .declaration = std::move(node),
    };
}

VnlcExportDeclarationParsingResult VnlcParser::parseExportDeclaration() {
    VnlcToken firstToken = peek();

    if (!match(VnlcTokenType::EXPORT)) {
        throw VnlcSyntaxError("Expected 'export' keyword", peek().getLine(), peek().getColumn());
    }
    skipNewlines();

    auto result = parseExportList();

    VnlcToken lastToken = peek();

    std::unique_ptr<VnlcExportDeclarationNode> node = std::make_unique<VnlcExportDeclarationNode>(std::move(result.items), firstToken, lastToken);

    return VnlcExportDeclarationParsingResult{
        .declaration = std::move(node),
    };
}

VnlcVariableDeclarationParsingResult VnlcParser::parseVariableDeclaration(VnlcVariableDeclarationParsingContext context) {
    VnlcToken firstToken = peek();

    auto primaryResult = parseVariableDeclarationPrimary();

    if (!match(VnlcTokenType::EQUAL)) {
        throw VnlcSyntaxError("Expected '=' after variable declaration", peek().getLine(), peek().getColumn());
    }

    auto initializerResult = parseExpression();

    VnlcToken lastToken = peek();

    std::unique_ptr<VnlcVariableDeclarationNode> node = nullptr;
    if (context.hasMetadata) {
        node = std::make_unique<VnlcVariableDeclarationNode>(
            primaryResult.type,
            std::move(primaryResult.name),
            std::move(primaryResult.typeAnnotation),
            std::move(initializerResult.expression),
            firstToken,
            lastToken,
            std::move(context.metadataTerms)
        );
    } else {
        node = std::make_unique<VnlcVariableDeclarationNode>(
            primaryResult.type,
            std::move(primaryResult.name),
            std::move(primaryResult.typeAnnotation),
            std::move(initializerResult.expression),
            firstToken,
            lastToken
        );
    };

    return VnlcVariableDeclarationParsingResult{
        .declaration = std::move(node),
    };
}
#include "VnlcParser.hpp"
#include "../ast/declaration/VnlcDeclarationNode.hpp"
#include "../ast/declaration/VnlcExportDeclarationNode.hpp"
#include "../ast/declaration/VnlcImportDeclarationNode.hpp"
#include "../ast/module/VnlcModuleNode.hpp"
#include "../error/VnlcIllegalModuleOrPackageNameError.hpp"
#include "../error/VnlcInternalError.hpp"
#include "../error/VnlcOutOfRangeError.hpp"
#include "../error/VnlcSyntaxError.hpp"
#include "../util/VnlcTokenTypeUtil.hpp"
#include "synthesized/VnlcClassMethodDeclarationParsingResult.hpp"
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

bool VnlcParser::checkGeneralizedIdentifier() {
    if (VnlcTokenTypeUtil::isGeneralizedIdentifier(peek().getType())) {
        return true;
    }
    return false;
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
    while (std::getline(ss, namePart, static_cast<char>(std::filesystem::path::preferred_separator))) {
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
        auto result = parseVariableDeclaration(std::move(context));

        VnlcToken lastToken = peek();
        result.declaration->resetPosition(firstToken, lastToken);

        if (!matchSeparatorEndOfLine()) {
            throw VnlcSyntaxError("Expected newline after variable declaration", peek().getLine(), peek().getColumn());
        }

        return VnlcTopIdentifierDeclarationParsingResult{
            .declaration = std::move(result.declaration),
        };
    } else if (check(VnlcTokenType::FUNC) || check(VnlcTokenType::NATIVE)) {
        VnlcFunctionDeclarationParsingContext context{
            .context = VnlcFunctionDeclarationType::Context::TOP_LEVEL,
            .accessModifier = VnlcFunctionDeclarationType::AccessModifier::PUBLIC,
            .binding = VnlcFunctionDeclarationType::Binding::STATIC,
            .hasMetadata = hasMetadata,
            .metadataTerms = std::move(metadataTerms),
        };
        auto result = parseFunctionDeclaration(std::move(context));

        VnlcToken lastToken = peek();
        result.declaration->resetPosition(firstToken, lastToken);

        if (!matchSeparatorEndOfLine()) {
            throw VnlcSyntaxError("Expected newline after function declaration", peek().getLine(), peek().getColumn());
        }

        return VnlcTopIdentifierDeclarationParsingResult{
            .declaration = std::move(result.declaration),
        };
    } else if (check(VnlcTokenType::CLASS) || check(VnlcTokenType::INTERFACE) || check(VnlcTokenType::ENUM) || check(VnlcTokenType::TYPE) || check(VnlcTokenType::FINAL)) {
        VnlcTypeDeclarationParsingContext context{
            .hasMetadata = hasMetadata,
            .metadataTerms = std::move(metadataTerms),
        };
        auto result = parseTypeDeclaration(std::move(context));

        VnlcToken lastToken = peek();
        result.declaration->resetPosition(firstToken, lastToken);

        if (!matchSeparatorEndOfLine()) {
            throw VnlcSyntaxError("Expected newline after type declaration", peek().getLine(), peek().getColumn());
        }

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
    skipNewlines();

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

VnlcFunctionDeclarationParsingResult VnlcParser::parseFunctionDeclaration(VnlcFunctionDeclarationParsingContext context) {
    VnlcToken firstToken = peek();

    if (check(VnlcTokenType::FUNC)) {
        VnlcRegularFunctionDeclarationParsingContext regularContext{
            .context = context.context,
            .hasMetadata = context.hasMetadata,
            .metadataTerms = std::move(context.metadataTerms),
        };
        auto result = parseRegularFunctionDeclaration(std::move(regularContext));
        result.declaration->resetPosition(firstToken, peek());

        VnlcToken lastToken = peek();

        return VnlcFunctionDeclarationParsingResult{
            .declaration = std::move(result.declaration),
        };
    } else if (check(VnlcTokenType::NATIVE)) {
        VnlcNativeFunctionDeclarationParsingContext nativeContext{
            .context = context.context,
            .hasMetadata = context.hasMetadata,
            .metadataTerms = std::move(context.metadataTerms),
        };
        auto result = parseNativeFunctionDeclaration(std::move(nativeContext));

        VnlcToken lastToken = peek();
        result.declaration->resetPosition(firstToken, lastToken);

        return VnlcFunctionDeclarationParsingResult{
            .declaration = std::move(result.declaration),
        };
    } else {
        throw VnlcSyntaxError("Expected 'func' or 'native' keyword", peek().getLine(), peek().getColumn());
    }
}

VnlcTypeDeclarationParsingResult VnlcParser::parseTypeDeclaration(VnlcTypeDeclarationParsingContext context) {
    VnlcToken firstToken = peek();

    if (check(VnlcTokenType::CLASS) || check(VnlcTokenType::FINAL)) {
        VnlcClassDeclarationParsingContext classContext{
            .hasMetadata = context.hasMetadata,
            .metadataTerms = std::move(context.metadataTerms),
        };

        auto result = parseClassDeclaration(std::move(classContext));

        VnlcToken lastToken = peek();

        return VnlcTypeDeclarationParsingResult{
            .declaration = std::move(result.declaration),
        };
    } else if (check(VnlcTokenType::INTERFACE)) {
        VnlcInterfaceDeclarationParsingContext interfaceContext{
            .hasMetadata = context.hasMetadata,
            .metadataTerms = std::move(context.metadataTerms),
        };

        auto result = parseInterfaceDeclaration(std::move(interfaceContext));

        VnlcToken lastToken = peek();

        return VnlcTypeDeclarationParsingResult{
            .declaration = std::move(result.declaration),
        };
    } else if (check(VnlcTokenType::ENUM)) {
        VnlcEnumDeclarationParsingContext enumContext{
            .hasMetadata = context.hasMetadata,
            .metadataTerms = std::move(context.metadataTerms),
        };

        auto result = parseEnumDeclaration(std::move(enumContext));

        VnlcToken lastToken = peek();

        return VnlcTypeDeclarationParsingResult{
            .declaration = std::move(result.declaration),
        };
    } else if (check(VnlcTokenType::TYPE)) {
        VnlcTypeAliasDeclarationParsingContext typeAliasContext{
            .hasMetadata = context.hasMetadata,
            .metadataTerms = std::move(context.metadataTerms),
        };

        auto result = parseTypeAliasDeclaration(std::move(typeAliasContext));

        VnlcToken lastToken = peek();

        return VnlcTypeDeclarationParsingResult{
            .declaration = std::move(result.declaration),
        };
    } else {
        throw VnlcSyntaxError("Expected 'class', 'interface', 'enum' or 'type' keyword", peek().getLine(), peek().getColumn());
    }
}

VnlcPropertyDeclarationParsingResult VnlcParser::parsePropertyDeclaration(VnlcPropertyDeclarationParsingContext context) {
    VnlcPropertyDeclarationType::AccessModifier accessModifier = VnlcPropertyDeclarationType::AccessModifier::PUBLIC;
    VnlcPropertyDeclarationType::Binding binding = VnlcPropertyDeclarationType::Binding::INSTANCE;

    VnlcToken firstToken = peek();

    if (match(VnlcTokenType::PRIVATE)) {
        accessModifier = VnlcPropertyDeclarationType::AccessModifier::PRIVATE;
        skipNewlines();
    } else if (match(VnlcTokenType::PUBLIC)) {
        accessModifier = VnlcPropertyDeclarationType::AccessModifier::PUBLIC;
        skipNewlines();
    }

    if (match(VnlcTokenType::STATIC)) {
        binding = VnlcPropertyDeclarationType::Binding::STATIC;
        skipNewlines();
    }

    std::string name;
    if (!check(VnlcTokenType::IDENTIFIER)) {
        throw VnlcSyntaxError("Expected property name", peek().getLine(), peek().getColumn());
    } else {
        name = peek().getValue();
        advance();
    }
    skipNewlines();

    if (!match(VnlcTokenType::COLON)) {
        throw VnlcSyntaxError("Expected ':' after property name", peek().getLine(), peek().getColumn());
    }
    skipNewlines();

    auto typeAnnotationResult = parseTypeAnnotation();

    if (match(VnlcTokenType::EQUAL)) {
        skipNewlines();

        auto initializerResult = parseExpression();

        VnlcToken lastToken = peek();

        std::unique_ptr<VnlcPropertyDeclarationNode> node;

        if (context.hasMetadata) {
            node = std::make_unique<VnlcPropertyDeclarationNode>(
                accessModifier,
                binding,
                std::move(name),
                std::move(typeAnnotationResult.typeAnnotation),
                std::make_optional<std::unique_ptr<VnlcExpressionNode>>(std::move(initializerResult.expression)),
                firstToken,
                lastToken,
                std::move(context.metadataTerms)
            );
        } else {
            node = std::make_unique<VnlcPropertyDeclarationNode>(
                accessModifier,
                binding,
                std::move(name),
                std::move(typeAnnotationResult.typeAnnotation),
                std::make_optional<std::unique_ptr<VnlcExpressionNode>>(std::move(initializerResult.expression)),
                firstToken,
                lastToken
            );
        }

        return VnlcPropertyDeclarationParsingResult{
            .declaration = std::move(node),
        };
    } else {
        VnlcToken lastToken = peek();

        std::unique_ptr<VnlcPropertyDeclarationNode> node;

        if (context.hasMetadata) {
            node = std::make_unique<VnlcPropertyDeclarationNode>(
                accessModifier,
                binding,
                std::move(name),
                std::move(typeAnnotationResult.typeAnnotation),
                std::nullopt,
                firstToken,
                lastToken,
                std::move(context.metadataTerms)
            );
        } else {
            node = std::make_unique<VnlcPropertyDeclarationNode>(accessModifier, binding, std::move(name), std::move(typeAnnotationResult.typeAnnotation), std::nullopt, firstToken, lastToken);
        }

        return VnlcPropertyDeclarationParsingResult{
            .declaration = std::move(node),
        };
    }
}

VnlcClassMethodDeclarationParsingResult VnlcParser::parseClassMethodDeclaration(VnlcClassMethodDeclarationParsingContext context) {
    VnlcFunctionDeclarationType::AccessModifier accessModifier = VnlcFunctionDeclarationType::AccessModifier::PUBLIC;
    VnlcFunctionDeclarationType::Binding binding = VnlcFunctionDeclarationType::Binding::INSTANCE;

    VnlcToken firstToken = peek();

    if (match(VnlcTokenType::PRIVATE)) {
        accessModifier = VnlcFunctionDeclarationType::AccessModifier::PRIVATE;
        skipNewlines();
    } else if (match(VnlcTokenType::PUBLIC)) {
        accessModifier = VnlcFunctionDeclarationType::AccessModifier::PUBLIC;
        skipNewlines();
    }

    if (match(VnlcTokenType::STATIC)) {
        binding = VnlcFunctionDeclarationType::Binding::STATIC;
        skipNewlines();
    } else if (match(VnlcTokenType::OVERRIDE)) {
        binding = VnlcFunctionDeclarationType::Binding::INSTANCE;
        skipNewlines();
    }

    VnlcFunctionDeclarationParsingContext functionDeclarationContext{
        .context = VnlcFunctionDeclarationType::Context::CLASS,
        .accessModifier = accessModifier,
        .binding = binding,
        .hasMetadata = context.hasMetadata,
        .metadataTerms = std::move(context.metadataTerms),
    };

    auto result = parseFunctionDeclaration(std::move(functionDeclarationContext));

    VnlcToken lastToken = peek();

    return VnlcClassMethodDeclarationParsingResult{
        .declaration = std::move(result.declaration),
    };
}

VnlcInterfaceMethodDeclarationParsingResult VnlcParser::parseInterfaceMethodDeclaration() {
    VnlcToken firstToken = peek();

    bool hasMetadata = false;
    std::vector<VnlcDeclarationItem::MetadataTerm> metadataTerms;
    if (check(VnlcTokenType::METADATA)) {
        hasMetadata = true;
        auto metadataResult = parseMetadata();
        metadataTerms = std::move(metadataResult.metadata);
    }

    auto result = parseFunctionSignature();

    VnlcToken lastToken = peek();

    if (!matchSeparatorEndOfLine()) {
        throw VnlcSyntaxError("Expected newline after interface method declaration", peek().getLine(), peek().getColumn());
    }

    std::unique_ptr<VnlcFunctionDeclarationNode> node;
    if (hasMetadata) {
        node = std::make_unique<VnlcFunctionDeclarationNode>(
            VnlcFunctionDeclarationType::Kind::REGULAR,
            VnlcFunctionDeclarationType::Context::INTERFACE,
            VnlcFunctionDeclarationType::AccessModifier::PUBLIC,
            VnlcFunctionDeclarationType::Binding::INSTANCE,
            std::move(result.name),
            std::move(result.parameters),
            std::move(result.returnType),
            std::nullopt,
            firstToken,
            lastToken,
            std::move(metadataTerms)
        );
    } else {
        node = std::make_unique<VnlcFunctionDeclarationNode>(
            VnlcFunctionDeclarationType::Kind::REGULAR,
            VnlcFunctionDeclarationType::Context::INTERFACE,
            VnlcFunctionDeclarationType::AccessModifier::PUBLIC,
            VnlcFunctionDeclarationType::Binding::INSTANCE,
            std::move(result.name),
            std::move(result.parameters),
            std::move(result.returnType),
            std::nullopt,
            firstToken,
            lastToken
        );
    }

    return VnlcInterfaceMethodDeclarationParsingResult{
        .declaration = std::move(node),
    };
}

VnlcMetadataParsingResult VnlcParser::parseMetadata() {
    VnlcToken firstToken = peek();

    if (!match(VnlcTokenType::METADATA)) {
        throw VnlcSyntaxError("Expected 'metadata' keyword", peek().getLine(), peek().getColumn());
    }
    skipNewlines();

    if (!match(VnlcTokenType::LEFT_PARENTHESIS)) {
        throw VnlcSyntaxError("Expected '(' after 'metadata' keyword", peek().getLine(), peek().getColumn());
    }
    skipNewlines();

    std::vector<VnlcDeclarationItem::MetadataTerm> metadataTerms;

    do {
        skipNewlines();
        auto result = parseMetadataTerm();
        metadataTerms.push_back(std::move(result.term));

    } while (match(VnlcTokenType::COMMA));

    if (!match(VnlcTokenType::RIGHT_PARENTHESIS)) {
        throw VnlcSyntaxError("Expected ')' after metadata terms", peek().getLine(), peek().getColumn());
    }
    skipNewlines();

    VnlcToken lastToken = peek();

    return VnlcMetadataParsingResult{
        .metadata = std::move(metadataTerms),
    };
}
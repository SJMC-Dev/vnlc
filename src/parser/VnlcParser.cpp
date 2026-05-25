#include "VnlcParser.hpp"
#include "../ast/declaration/VnlcDeclarationNode.hpp"
#include "../ast/declaration/VnlcExportDeclarationNode.hpp"
#include "../ast/declaration/VnlcImportDeclarationNode.hpp"
#include "../ast/expression/VnlcBinaryExpressionNode.hpp"
#include "../ast/expression/VnlcBinaryExpressionType.hpp"
#include "../ast/expression/VnlcConditionalExpressionNode.hpp"
#include "../ast/expression/VnlcDictLiteralExpressionNode.hpp"
#include "../ast/expression/VnlcFunctionCallExpressionNode.hpp"
#include "../ast/expression/VnlcListLikeLiteralExpressionNode.hpp"
#include "../ast/expression/VnlcListLikeLiteralExpressionType.hpp"
#include "../ast/expression/VnlcMemberAccessExpressionNode.hpp"
#include "../ast/expression/VnlcMemberAccessExpressionType.hpp"
#include "../ast/expression/VnlcRangeExpressionNode.hpp"
#include "../ast/expression/VnlcSelectorLiteralExpressionNode.hpp"
#include "../ast/expression/VnlcSelectorLiteralExpressionType.hpp"
#include "../ast/expression/VnlcSimpleLiteralExpressionNode.hpp"
#include "../ast/expression/VnlcStringLiteralExpressionNode.hpp"
#include "../ast/expression/VnlcStringLiteralExpressionType.hpp"
#include "../ast/expression/VnlcSubscriptExpressionNode.hpp"
#include "../ast/expression/VnlcSuperExpressionNode.hpp"
#include "../ast/expression/VnlcThisExpressionNode.hpp"
#include "../ast/expression/VnlcUnaryExpressionNode.hpp"
#include "../ast/expression/VnlcUnaryExpressionType.hpp"
#include "../ast/module/VnlcModuleNode.hpp"
#include "../ast/statement/VnlcSwitchStatementItem.hpp"
#include "../ast/statement/VnlcSwitchStatementNode.hpp"
#include "../ast/statement/VnlcSwitchStatementType.hpp"
#include "../error/VnlcIllegalModuleOrPackageNameError.hpp"
#include "../error/VnlcInternalError.hpp"
#include "../error/VnlcOutOfRangeError.hpp"
#include "../error/VnlcSyntaxError.hpp"
#include "../util/VnlcTokenTypeUtil.hpp"
#include <memory>
#include <optional>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

VnlcParser::VnlcParser(VnlcLexer&& lexer, unsigned int maxBufferSize) : lexer(std::move(lexer)), tokenBuffer(), currentTokenIndex(0), bufferSize(maxBufferSize) {
    fillBuffer();
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

const VnlcToken& VnlcParser::peekValid() {
    skipNewlines();

    return peek();
}

void VnlcParser::fillBuffer() {
    bool blank = false;
    tokenBuffer.clear();

    for (unsigned int i = 0; i < bufferSize && lexer.hasNext(); i = blank ? i : i + 1) {
        VnlcToken token = lexer.next();
        if (token.getType() == VnlcTokenType::BLANK || token.getType() == VnlcTokenType::SINGLE_LINE_COMMENT || token.getType() == VnlcTokenType::MULTI_LINE_COMMENT) {
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
        currentTokenIndex = bufferSize; // Move index to the end of the buffer to indicate no more tokens
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
    while (hasNextToken() && peek().getType() == VnlcTokenType::NEWLINE) {
        advance();
    }
}

bool VnlcParser::check(VnlcTokenType expectedType) {
    skipNewlines();
    return hasNextToken() && peek().getType() == expectedType;
}

bool VnlcParser::checkGeneralizedIdentifier() {
    skipNewlines();
    if (hasNextToken() && VnlcTokenTypeUtil::isGeneralizedIdentifier(peek().getType())) {
        return true;
    }
    return false;
}

bool VnlcParser::checkAny(const std::unordered_set<VnlcTokenType>& expectedTypes) {
    skipNewlines();
    return hasNextToken() && expectedTypes.contains(peek().getType());
}

bool VnlcParser::match(VnlcTokenType expectedType) {
    if (check(expectedType)) {
        advance();
        return true;
    }
    return false;
}

bool VnlcParser::matchSeparatorEndOfLine() {
    if (hasNextToken() && peek().getType() == VnlcTokenType::NEWLINE) {
        skipNewlines();
        return true;
    } else if (hasNextToken() && peek().getType() == VnlcTokenType::END_OF_FILE) {
        return true;
    }
    return false;
}

bool VnlcParser::matchAny(const std::unordered_set<VnlcTokenType>& expectedTypes) {
    if (checkAny(expectedTypes)) {
        advance();
        return true;
    }
    return false;
}

bool VnlcParser::consumeRightAngleInType() {
    if (match(VnlcTokenType::RIGHT_ANGLE)) {
        return true;
    }

    if (check(VnlcTokenType::DOUBLE_RIGHT_ANGLE)) {
        tokenBuffer[currentTokenIndex] = VnlcToken(VnlcTokenType::RIGHT_ANGLE, ">", peek().getLine(), peek().getColumn() + 1, peek().getOffset() + 1);
        return true;
    }

    if (check(VnlcTokenType::TRIPLE_RIGHT_ANGLE)) {
        tokenBuffer[currentTokenIndex] = VnlcToken(VnlcTokenType::DOUBLE_RIGHT_ANGLE, ">>", peek().getLine(), peek().getColumn() + 1, peek().getOffset() + 1);
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

    std::string prefix = context.config.packageRootPath.string();
    std::string fullPath = context.config.inputFilePath.string();

    if (!fullPath.starts_with(prefix)) {
        throw VnlcInternalError("Module file path is outside the package root");
    }
    fullPath.erase(0, prefix.length());
    if (fullPath.starts_with(std::filesystem::path::preferred_separator)) {
        fullPath.erase(0, 1);
    }

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

    VnlcToken firstToken = peekValid();

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

    VnlcToken firstToken = peekValid();

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
    VnlcToken firstToken = peekValid();

    if (!match(VnlcTokenType::IMPORT)) {
        throw VnlcSyntaxError("Expected 'import' keyword", peek().getLine(), peek().getColumn());
    }

    auto result = parseImportPath();

    VnlcToken lastToken = peek();

    std::unique_ptr<VnlcImportDeclarationNode> node = std::make_unique<VnlcImportDeclarationNode>(result.relative, std::move(result.paths), firstToken, lastToken);

    if (!matchSeparatorEndOfLine()) {
        throw VnlcSyntaxError("Expected newline after import declaration", peek().getLine(), peek().getColumn());
    }

    return VnlcImportDeclarationParsingResult{
        .declaration = std::move(node),
    };
}

VnlcExportDeclarationParsingResult VnlcParser::parseExportDeclaration() {
    VnlcToken firstToken = peekValid();

    if (!match(VnlcTokenType::EXPORT)) {
        throw VnlcSyntaxError("Expected 'export' keyword", peek().getLine(), peek().getColumn());
    }

    auto result = parseExportList();

    VnlcToken lastToken = peek();

    std::unique_ptr<VnlcExportDeclarationNode> node = std::make_unique<VnlcExportDeclarationNode>(std::move(result.items), firstToken, lastToken);

    if (!matchSeparatorEndOfLine()) {
        throw VnlcSyntaxError("Expected newline after export declaration", peek().getLine(), peek().getColumn());
    }

    return VnlcExportDeclarationParsingResult{
        .declaration = std::move(node),
    };
}

VnlcVariableDeclarationParsingResult VnlcParser::parseVariableDeclaration(VnlcVariableDeclarationParsingContext context) {
    VnlcToken firstToken = peekValid();

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

VnlcFunctionDeclarationParsingResult VnlcParser::parseFunctionDeclaration(VnlcFunctionDeclarationParsingContext context) {
    if (check(VnlcTokenType::FUNC)) {
        VnlcRegularFunctionDeclarationParsingContext regularContext{
            .context = context.context,
            .hasMetadata = context.hasMetadata,
            .metadataTerms = std::move(context.metadataTerms),
        };
        auto result = parseRegularFunctionDeclaration(std::move(regularContext));

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

        return VnlcFunctionDeclarationParsingResult{
            .declaration = std::move(result.declaration),
        };
    } else {
        throw VnlcSyntaxError("Expected 'func' or 'native' keyword", peek().getLine(), peek().getColumn());
    }
}

VnlcTypeDeclarationParsingResult VnlcParser::parseTypeDeclaration(VnlcTypeDeclarationParsingContext context) {
    VnlcToken firstToken = peekValid();

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
    VnlcToken firstToken = peekValid();

    std::string name;
    if (!check(VnlcTokenType::IDENTIFIER)) {
        throw VnlcSyntaxError("Expected property name", peek().getLine(), peek().getColumn());
    } else {
        name = peek().getValue();
        advance();
    }

    if (!match(VnlcTokenType::COLON)) {
        throw VnlcSyntaxError("Expected ':' after property name", peek().getLine(), peek().getColumn());
    }

    auto typeAnnotationResult = parseTypeAnnotation();

    if (match(VnlcTokenType::EQUAL)) {
        auto initializerResult = parseExpression();

        VnlcToken lastToken = peek();

        std::unique_ptr<VnlcPropertyDeclarationNode> node;

        if (context.hasMetadata) {
            node = std::make_unique<VnlcPropertyDeclarationNode>(
                context.accessModifier,
                context.binding,
                std::move(name),
                std::move(typeAnnotationResult.typeAnnotation),
                std::make_optional<std::unique_ptr<VnlcExpressionNode>>(std::move(initializerResult.expression)),
                firstToken,
                lastToken,
                std::move(context.metadataTerms)
            );
        } else {
            node = std::make_unique<VnlcPropertyDeclarationNode>(
                context.accessModifier,
                context.binding,
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
                context.accessModifier,
                context.binding,
                std::move(name),
                std::move(typeAnnotationResult.typeAnnotation),
                std::nullopt,
                firstToken,
                lastToken,
                std::move(context.metadataTerms)
            );
        } else {
            node = std::make_unique<
                VnlcPropertyDeclarationNode>(context.accessModifier, context.binding, std::move(name), std::move(typeAnnotationResult.typeAnnotation), std::nullopt, firstToken, lastToken);
        }

        return VnlcPropertyDeclarationParsingResult{
            .declaration = std::move(node),
        };
    }
}

VnlcInterfaceMethodDeclarationParsingResult VnlcParser::parseInterfaceMethodDeclaration() {
    VnlcToken firstToken = peekValid();

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
    if (!match(VnlcTokenType::METADATA)) {
        throw VnlcSyntaxError("Expected 'metadata' keyword", peek().getLine(), peek().getColumn());
    }

    if (!match(VnlcTokenType::LEFT_PARENTHESIS)) {
        throw VnlcSyntaxError("Expected '(' after 'metadata' keyword", peek().getLine(), peek().getColumn());
    }

    std::vector<VnlcDeclarationItem::MetadataTerm> metadataTerms;

    do {
        auto result = parseMetadataTerm();
        metadataTerms.push_back(std::move(result.term));

    } while (match(VnlcTokenType::COMMA));

    if (!match(VnlcTokenType::RIGHT_PARENTHESIS)) {
        throw VnlcSyntaxError("Expected ')' after metadata terms", peek().getLine(), peek().getColumn());
    }

    return VnlcMetadataParsingResult{
        .metadata = std::move(metadataTerms),
    };
}

VnlcVariableDeclarationPrimaryParsingResult VnlcParser::parseVariableDeclarationPrimary() {
    VnlcVariableDeclarationType type;
    std::string name;
    std::optional<std::unique_ptr<VnlcTypeAnnotationNode>> typeAnnotation = std::nullopt;

    if (match(VnlcTokenType::VAR)) {
        type = VnlcVariableDeclarationType::VAR;
    } else if (match(VnlcTokenType::LET)) {
        type = VnlcVariableDeclarationType::LET;
    } else if (match(VnlcTokenType::CONST)) {
        type = VnlcVariableDeclarationType::CONST;
    } else {
        throw VnlcSyntaxError("Expected 'var', 'let' or 'const' keyword", peek().getLine(), peek().getColumn());
    }

    if (!check(VnlcTokenType::IDENTIFIER)) {
        throw VnlcSyntaxError("Expected variable name", peek().getLine(), peek().getColumn());
    } else {
        name = peek().getValue();
        advance();
    }

    if (match(VnlcTokenType::COLON)) {

        auto typeAnnotationResult = parseTypeAnnotation();
        typeAnnotation = std::make_optional<std::unique_ptr<VnlcTypeAnnotationNode>>(std::move(typeAnnotationResult.typeAnnotation));
    }

    return VnlcVariableDeclarationPrimaryParsingResult{
        .type = type,
        .name = std::move(name),
        .typeAnnotation = std::move(typeAnnotation),
    };
}

VnlcRegularFunctionDeclarationParsingResult VnlcParser::parseRegularFunctionDeclaration(VnlcRegularFunctionDeclarationParsingContext context) {
    VnlcToken firstToken = peekValid();

    auto signatureResult = parseFunctionSignature();
    auto bodyResult = parseFunctionBody();

    VnlcToken lastToken = peek();

    if (context.hasMetadata) {
        return VnlcRegularFunctionDeclarationParsingResult{
            .declaration = std::make_unique<VnlcFunctionDeclarationNode>(
                VnlcFunctionDeclarationType::Kind::REGULAR,
                context.context,
                context.accessModifier,
                context.binding,
                std::move(signatureResult.name),
                std::move(signatureResult.parameters),
                std::move(signatureResult.returnType),
                std::move(bodyResult.body),
                firstToken,
                lastToken,
                std::move(context.metadataTerms)
            ),
        };
    } else {
        return VnlcRegularFunctionDeclarationParsingResult{
            .declaration = std::make_unique<VnlcFunctionDeclarationNode>(
                VnlcFunctionDeclarationType::Kind::REGULAR,
                context.context,
                context.accessModifier,
                context.binding,
                std::move(signatureResult.name),
                std::move(signatureResult.parameters),
                std::move(signatureResult.returnType),
                std::move(bodyResult.body),
                firstToken,
                lastToken
            ),
        };
    }
}

VnlcNativeFunctionDeclarationParsingResult VnlcParser::parseNativeFunctionDeclaration(VnlcNativeFunctionDeclarationParsingContext context) {
    VnlcToken firstToken = peekValid();

    if (!match(VnlcTokenType::NATIVE)) {
        throw VnlcSyntaxError("Expected 'native' keyword", peek().getLine(), peek().getColumn());
    }

    auto signatureResult = parseFunctionSignature();

    VnlcToken lastToken = peek();

    if (context.hasMetadata) {
        return VnlcNativeFunctionDeclarationParsingResult{
            .declaration = std::make_unique<VnlcFunctionDeclarationNode>(
                VnlcFunctionDeclarationType::Kind::NATIVE,
                context.context,
                context.accessModifier,
                context.binding,
                std::move(signatureResult.name),
                std::move(signatureResult.parameters),
                std::move(signatureResult.returnType),
                std::nullopt,
                firstToken,
                lastToken,
                std::move(context.metadataTerms)
            ),
        };
    } else {
        return VnlcNativeFunctionDeclarationParsingResult{
            .declaration = std::make_unique<VnlcFunctionDeclarationNode>(
                VnlcFunctionDeclarationType::Kind::NATIVE,
                context.context,
                context.accessModifier,
                context.binding,
                std::move(signatureResult.name),
                std::move(signatureResult.parameters),
                std::move(signatureResult.returnType),
                std::nullopt,
                firstToken,
                lastToken
            ),
        };
    }
}

VnlcTypeAnnotationParsingResult VnlcParser::parseTypeAnnotation() {
    VnlcToken firstToken = peekValid();

    bool readonly = false;

    if (match(VnlcTokenType::READONLY)) {
        readonly = true;
    }

    auto result = parseType();

    VnlcToken lastToken = peek();

    return VnlcTypeAnnotationParsingResult{
        .typeAnnotation = std::make_unique<VnlcTypeAnnotationNode>(readonly, std::move(result.type), firstToken, lastToken),
    };
}

VnlcParameterListParsingResult VnlcParser::parseParameterList() {
    std::vector<VnlcParameterListParsingResult::Parameter> parameters;

    do {
        auto result = parseParameter();
        parameters.emplace_back(
            VnlcParameterListParsingResult::Parameter{
                .name = std::move(result.name),
                .typeAnnotation = std::move(result.typeAnnotation),
            }
        );
    } while (match(VnlcTokenType::COMMA));

    return VnlcParameterListParsingResult{
        .parameters = std::move(parameters),
    };
}

VnlcClassDeclarationParsingResult VnlcParser::parseClassDeclaration(VnlcClassDeclarationParsingContext context) {
    bool final = false;
    std::string name;
    std::optional<std::unique_ptr<VnlcTypeNode>> baseClass = std::nullopt;
    std::vector<std::unique_ptr<VnlcTypeNode>> implementedInterfaces;
    std::vector<std::string> genericParameterNames;
    std::vector<std::unique_ptr<VnlcDeclarationNode>> memberDeclarations;

    VnlcToken firstToken = peekValid();

    if (match(VnlcTokenType::FINAL)) {
        final = true;
    }

    if (!match(VnlcTokenType::CLASS)) {
        throw VnlcSyntaxError("Expected 'class' keyword", peek().getLine(), peek().getColumn());
    }

    if (!check(VnlcTokenType::IDENTIFIER)) {
        throw VnlcSyntaxError("Expected class name", peek().getLine(), peek().getColumn());
    } else {
        name = peek().getValue();
        advance();
    }

    if (match(VnlcTokenType::LEFT_ANGLE)) {

        auto genericParameterListResult = parseGenericParameterList();
        genericParameterNames = std::move(genericParameterListResult.parameters);

        if (!match(VnlcTokenType::RIGHT_ANGLE)) {
            throw VnlcSyntaxError("Expected '>' after generic parameter list", peek().getLine(), peek().getColumn());
        }
    }

    if (match(VnlcTokenType::EXTENDS)) {

        auto typeResult = parseType();
        baseClass = std::make_optional<std::unique_ptr<VnlcTypeNode>>(std::move(typeResult.type));
    }

    if (match(VnlcTokenType::IMPLEMENTS)) {
        do {
            auto typeResult = parseType();
            implementedInterfaces.push_back(std::move(typeResult.type));
        } while (match(VnlcTokenType::COMMA));
    }

    auto bodyResult = parseClassBody();

    VnlcToken lastToken = peek();

    if (context.hasMetadata) {
        return VnlcClassDeclarationParsingResult{
            .declaration = std::make_unique<VnlcClassDeclarationNode>(
                final,
                std::move(name),
                std::move(baseClass),
                std::move(implementedInterfaces),
                std::move(genericParameterNames),
                std::move(bodyResult.declarations),
                firstToken,
                lastToken,
                std::move(context.metadataTerms)
            ),
        };
    } else {
        return VnlcClassDeclarationParsingResult{
            .declaration = std::make_unique<VnlcClassDeclarationNode>(
                final,
                std::move(name),
                std::move(baseClass),
                std::move(implementedInterfaces),
                std::move(genericParameterNames),
                std::move(bodyResult.declarations),
                firstToken,
                lastToken
            ),
        };
    }
}

VnlcInterfaceDeclarationParsingResult VnlcParser::parseInterfaceDeclaration(VnlcInterfaceDeclarationParsingContext context) {
    std::string name;
    std::vector<std::string> genericParameterNames;
    std::vector<std::unique_ptr<VnlcFunctionDeclarationNode>> methodDeclarations;

    VnlcToken firstToken = peekValid();

    if (!match(VnlcTokenType::INTERFACE)) {
        throw VnlcSyntaxError("Expected 'interface' keyword", peek().getLine(), peek().getColumn());
    }

    if (!check(VnlcTokenType::IDENTIFIER)) {
        throw VnlcSyntaxError("Expected interface name", peek().getLine(), peek().getColumn());
    } else {
        name = peek().getValue();
        advance();
    }

    if (match(VnlcTokenType::LEFT_ANGLE)) {
        auto genericParameterListResult = parseGenericParameterList();
        genericParameterNames = std::move(genericParameterListResult.parameters);

        if (!match(VnlcTokenType::RIGHT_ANGLE)) {
            throw VnlcSyntaxError("Expected '>' after generic parameter list", peek().getLine(), peek().getColumn());
        }
    }

    auto bodyResult = parseInterfaceBody();

    VnlcToken lastToken = peek();

    if (context.hasMetadata) {
        return VnlcInterfaceDeclarationParsingResult{
            .declaration = std::make_unique<
                VnlcInterfaceDeclarationNode>(std::move(name), std::move(genericParameterNames), std::move(bodyResult.declarations), firstToken, lastToken, std::move(context.metadataTerms)),
        };
    } else {
        return VnlcInterfaceDeclarationParsingResult{
            .declaration = std::make_unique<VnlcInterfaceDeclarationNode>(std::move(name), std::move(genericParameterNames), std::move(bodyResult.declarations), firstToken, lastToken),
        };
    }
}

VnlcEnumDeclarationParsingResult VnlcParser::parseEnumDeclaration(VnlcEnumDeclarationParsingContext context) {
    std::string name;
    std::vector<std::string> genericParameterNames;
    std::vector<std::unique_ptr<VnlcEnumMemberDeclarationNode>> memberDeclarations;

    VnlcToken firstToken = peekValid();

    if (!match(VnlcTokenType::ENUM)) {
        throw VnlcSyntaxError("Expected 'enum' keyword", peek().getLine(), peek().getColumn());
    }

    if (!check(VnlcTokenType::IDENTIFIER)) {
        throw VnlcSyntaxError("Expected enum name", peek().getLine(), peek().getColumn());
    } else {
        name = peek().getValue();
        advance();
    }

    if (match(VnlcTokenType::LEFT_ANGLE)) {
        auto genericParameterListResult = parseGenericParameterList();
        genericParameterNames = std::move(genericParameterListResult.parameters);

        if (!match(VnlcTokenType::RIGHT_ANGLE)) {
            throw VnlcSyntaxError("Expected '>' after generic parameter list", peek().getLine(), peek().getColumn());
        }
    }

    auto bodyResult = parseEnumBody();

    VnlcToken lastToken = peek();

    if (context.hasMetadata) {
        return VnlcEnumDeclarationParsingResult{
            .declaration = std::make_unique<
                VnlcEnumDeclarationNode>(std::move(name), std::move(genericParameterNames), std::move(bodyResult.declarations), firstToken, lastToken, std::move(context.metadataTerms)),
        };
    } else {
        return VnlcEnumDeclarationParsingResult{
            .declaration = std::make_unique<VnlcEnumDeclarationNode>(std::move(name), std::move(genericParameterNames), std::move(bodyResult.declarations), firstToken, lastToken),
        };
    }
}

VnlcTypeAliasDeclarationParsingResult VnlcParser::parseTypeAliasDeclaration(VnlcTypeAliasDeclarationParsingContext context) {
    std::string aliasName;
    std::vector<std::string> genericParameterNames;
    std::unique_ptr<VnlcTypeNode> originalType;

    VnlcToken firstToken = peekValid();

    if (!match(VnlcTokenType::TYPE)) {
        throw VnlcSyntaxError("Expected 'type' keyword", peek().getLine(), peek().getColumn());
    }

    if (!check(VnlcTokenType::IDENTIFIER)) {
        throw VnlcSyntaxError("Expected type alias name", peek().getLine(), peek().getColumn());
    } else {
        aliasName = peek().getValue();
        advance();
    }

    if (match(VnlcTokenType::LEFT_ANGLE)) {
        auto genericParameterListResult = parseGenericParameterList();
        genericParameterNames = std::move(genericParameterListResult.parameters);

        if (!match(VnlcTokenType::RIGHT_ANGLE)) {
            throw VnlcSyntaxError("Expected '>' after generic parameter list", peek().getLine(), peek().getColumn());
        }
    }

    if (!match(VnlcTokenType::EQUAL)) {
        throw VnlcSyntaxError("Expected '=' after type alias name", peek().getLine(), peek().getColumn());
    }

    auto typeResult = parseType();

    VnlcToken lastToken = peek();

    if (context.hasMetadata) {
        return VnlcTypeAliasDeclarationParsingResult{
            .declaration = std::make_unique<
                VnlcTypeAliasDeclarationNode>(std::move(aliasName), std::move(genericParameterNames), std::move(typeResult.type), firstToken, lastToken, std::move(context.metadataTerms)),
        };
    } else {
        return VnlcTypeAliasDeclarationParsingResult{
            .declaration = std::make_unique<VnlcTypeAliasDeclarationNode>(std::move(aliasName), std::move(genericParameterNames), std::move(typeResult.type), firstToken, lastToken),
        };
    }
}

VnlcImportPathParsingResult VnlcParser::parseImportPath() {
    bool relative = false;
    std::vector<VnlcImportDeclarationItem> paths;

    if (match(VnlcTokenType::MODULE)) {
        relative = true;

        if (!match(VnlcTokenType::DOT)) {
            throw VnlcSyntaxError("Expected '.' after 'module' keyword", peek().getLine(), peek().getColumn());
        }

        auto result = parseRelativeImportPath();
        paths = std::move(result.paths);
    } else {
        auto result = parseAbsoluteImportPath();
        paths = std::move(result.paths);
    }

    return VnlcImportPathParsingResult{
        .relative = relative,
        .paths = std::move(paths),
    };
}

VnlcExportListParsingResult VnlcParser::parseExportList() {
    std::vector<VnlcExportDeclarationItem> items;

    do {
        std::string name;
        std::optional<std::string> alias = std::nullopt;

        if (!check(VnlcTokenType::IDENTIFIER)) {
            throw VnlcSyntaxError("Expected identifier in export list", peek().getLine(), peek().getColumn());
        } else {
            name = peek().getValue();
            advance();
        }

        if (match(VnlcTokenType::AS)) {
            if (!check(VnlcTokenType::IDENTIFIER)) {
                throw VnlcSyntaxError("Expected identifier after 'as' keyword in export list", peek().getLine(), peek().getColumn());
            } else {
                alias = std::make_optional<std::string>(peek().getValue());
                advance();
            }
        }

        items.emplace_back(
            VnlcExportDeclarationItem{
                .name = std::move(name),
                .alias = std::move(alias),
            }
        );
    } while (match(VnlcTokenType::COMMA));

    return VnlcExportListParsingResult{
        .items = std::move(items),
    };
}

VnlcMetadataTermParsingResult VnlcParser::parseMetadataTerm() {
    std::string key;
    std::optional<std::string> value = std::nullopt;

    if (!checkGeneralizedIdentifier()) {
        throw VnlcSyntaxError("Expected metadata term key", peek().getLine(), peek().getColumn());
    } else {
        key = peek().getValue();
        advance();
    }

    if (check(VnlcTokenType::STRING)) {
        std::string literal(peek().getValue());

        if (!literal.starts_with('"') || !literal.ends_with('"')) {
            throw VnlcSyntaxError("Metadata term value must be a simple string literal", peek().getLine(), peek().getColumn());
        }

        value = std::make_optional<std::string>(literal.substr(1, literal.size() - 2));
        advance();
    }

    return VnlcMetadataTermParsingResult{
        .term =
            VnlcDeclarationItem::MetadataTerm{
                .key = std::move(key),
                .value = std::move(value),
            },
    };
}

VnlcFunctionSignatureParsingResult VnlcParser::parseFunctionSignature() {
    std::unique_ptr<std::string> name;
    std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>> parameters;
    std::optional<std::unique_ptr<VnlcTypeAnnotationNode>> returnType;

    if (!match(VnlcTokenType::FUNC)) {
        throw VnlcSyntaxError("Expected 'func' keyword", peek().getLine(), peek().getColumn());
    }

    if (!check(VnlcTokenType::IDENTIFIER)) {
        throw VnlcSyntaxError("Expected function name", peek().getLine(), peek().getColumn());
    } else {
        name = std::make_unique<std::string>(peek().getValue());
        advance();
    }

    if (!match(VnlcTokenType::LEFT_PARENTHESIS)) {
        throw VnlcSyntaxError("Expected '(' after function name", peek().getLine(), peek().getColumn());
    }

    if (!check(VnlcTokenType::RIGHT_PARENTHESIS)) {
        auto parameterListResult = parseParameterList();

        for (auto& parameter : parameterListResult.parameters) {
            parameters.emplace_back(std::move(parameter.name), std::move(parameter.typeAnnotation));
        }
    }

    if (!match(VnlcTokenType::RIGHT_PARENTHESIS)) {
        throw VnlcSyntaxError("Expected ')' after parameter list", peek().getLine(), peek().getColumn());
    }

    if (match(VnlcTokenType::ARROW)) {
        if (!match(VnlcTokenType::VOID)) {
            auto typeAnnotationResult = parseTypeAnnotation();
            returnType = std::make_optional<std::unique_ptr<VnlcTypeAnnotationNode>>(std::move(typeAnnotationResult.typeAnnotation));
        }
    }

    return VnlcFunctionSignatureParsingResult{
        .name = std::move(name),
        .parameters = std::move(parameters),
        .returnType = std::move(returnType),
    };
}

VnlcAbsoluteImportPathParsingResult VnlcParser::parseAbsoluteImportPath() {
    std::vector<VnlcImportDeclarationItem> paths;
    std::vector<std::string> namePartsPrefix;

    while (true) {
        if (!check(VnlcTokenType::IDENTIFIER)) {
            throw VnlcSyntaxError("Expected identifier in import path", peek().getLine(), peek().getColumn());
        } else {
            namePartsPrefix.emplace_back(peek().getValue());
            advance();
        }

        if (!check(VnlcTokenType::DOT)) {
            break;
        } else {
            advance();

            if (match(VnlcTokenType::ASTERISK)) {
                std::vector<std::string> nameParts = std::move(namePartsPrefix);
                nameParts.emplace_back("*");

                paths.emplace_back(
                    VnlcImportDeclarationItem{
                        .nameParts = std::move(nameParts),
                        .alias = std::nullopt,
                    }
                );

                return VnlcAbsoluteImportPathParsingResult{
                    .paths = std::move(paths),
                };
            } else {
                continue;
            }
        }
    };

    if (match(VnlcTokenType::AS)) {
        if (!check(VnlcTokenType::IDENTIFIER)) {
            throw VnlcSyntaxError("Expected identifier after 'as' keyword in import path", peek().getLine(), peek().getColumn());
        } else {
            std::string alias(peek().getValue());
            advance();

            paths.emplace_back(
                VnlcImportDeclarationItem{
                    .nameParts = std::move(namePartsPrefix),
                    .alias = std::make_optional<std::string>(std::move(alias)),
                }
            );
        }
    } else if (match(VnlcTokenType::LEFT_BRACE)) {

        auto listResult = parseAbsoluteImportPathList();

        for (auto& path : listResult.paths) {
            std::vector<std::string> nameParts = namePartsPrefix;
            nameParts.append_range(path.nameParts);

            paths.emplace_back(
                VnlcImportDeclarationItem{
                    .nameParts = std::move(nameParts),
                    .alias = std::move(path.alias),
                }
            );
        }
    } else {
        paths.emplace_back(
            VnlcImportDeclarationItem{
                .nameParts = std::move(namePartsPrefix),
                .alias = std::nullopt,
            }
        );
    }

    return VnlcAbsoluteImportPathParsingResult{
        .paths = std::move(paths),
    };
}

VnlcRelativeImportPathParsingResult VnlcParser::parseRelativeImportPath() {
    std::vector<VnlcImportDeclarationItem> paths;
    std::vector<std::string> namePartsPrefix;

    while (true) {
        if (match(VnlcTokenType::PARENT)) {
            namePartsPrefix.emplace_back("parent");
        } else if (!check(VnlcTokenType::IDENTIFIER)) {
            throw VnlcSyntaxError("Expected identifier or 'parent' in relative import path", peek().getLine(), peek().getColumn());
        } else {
            namePartsPrefix.emplace_back(peek().getValue());
            advance();
        }

        if (!check(VnlcTokenType::DOT)) {
            break;
        } else {
            advance();

            if (match(VnlcTokenType::ASTERISK)) {
                std::vector<std::string> nameParts = std::move(namePartsPrefix);
                nameParts.emplace_back("*");

                paths.emplace_back(
                    VnlcImportDeclarationItem{
                        .nameParts = std::move(nameParts),
                        .alias = std::nullopt,
                    }
                );

                return VnlcRelativeImportPathParsingResult{
                    .paths = std::move(paths),
                };
            } else {
                continue;
            }
        }
    }

    if (match(VnlcTokenType::AS)) {
        if (!check(VnlcTokenType::IDENTIFIER)) {
            throw VnlcSyntaxError("Expected identifier after 'as' keyword in import path", peek().getLine(), peek().getColumn());
        } else {
            std::string alias(peek().getValue());
            advance();

            paths.emplace_back(
                VnlcImportDeclarationItem{
                    .nameParts = std::move(namePartsPrefix),
                    .alias = std::make_optional<std::string>(std::move(alias)),
                }
            );
        }
    } else if (match(VnlcTokenType::LEFT_BRACE)) {
        auto listResult = parseRelativeImportPathList();

        for (auto& path : listResult.paths) {
            std::vector<std::string> nameParts = namePartsPrefix;
            nameParts.append_range(path.nameParts);

            paths.emplace_back(
                VnlcImportDeclarationItem{
                    .nameParts = std::move(nameParts),
                    .alias = std::move(path.alias),
                }
            );
        }
    } else {
        paths.emplace_back(
            VnlcImportDeclarationItem{
                .nameParts = std::move(namePartsPrefix),
                .alias = std::nullopt,
            }
        );
    }

    return VnlcRelativeImportPathParsingResult{
        .paths = std::move(paths),
    };
}

VnlcTypeParsingResult VnlcParser::parseType() {
    VnlcToken firstToken = peekValid();

    bool questionMarkSuffix = false;
    std::vector<std::string> nameParts;
    std::vector<std::unique_ptr<VnlcTypeNode>> genericArguments;

    do {
        if (!check(VnlcTokenType::IDENTIFIER)) {
            throw VnlcSyntaxError("Expected identifier in type", peek().getLine(), peek().getColumn());
        } else {
            nameParts.emplace_back(peek().getValue());
            advance();
        }
    } while (match(VnlcTokenType::DOT));

    if (match(VnlcTokenType::LEFT_ANGLE)) {
        auto genericArgumentListResult = parseGenericArgumentList();
        genericArguments = std::move(genericArgumentListResult.arguments);

        if (!consumeRightAngleInType()) {
            throw VnlcSyntaxError("Expected '>' after generic argument list in type", peek().getLine(), peek().getColumn());
        }
    }

    if (match(VnlcTokenType::QUESTION)) {
        questionMarkSuffix = true;
    }

    VnlcToken lastToken = peek();

    return VnlcTypeParsingResult{
        .type = std::make_unique<VnlcTypeNode>(questionMarkSuffix, std::move(nameParts), std::move(genericArguments), firstToken, lastToken),
    };
}

VnlcParameterParsingResult VnlcParser::parseParameter() {
    std::string name;
    std::unique_ptr<VnlcTypeAnnotationNode> typeAnnotation;

    if (!check(VnlcTokenType::IDENTIFIER)) {
        throw VnlcSyntaxError("Expected parameter name", peek().getLine(), peek().getColumn());
    } else {
        name = peek().getValue();
        advance();
    }

    if (!match(VnlcTokenType::COLON)) {
        throw VnlcSyntaxError("Expected ':' after parameter name", peek().getLine(), peek().getColumn());
    }

    auto typeAnnotationResult = parseTypeAnnotation();
    typeAnnotation = std::move(typeAnnotationResult.typeAnnotation);

    return VnlcParameterParsingResult{
        .name = std::move(name),
        .typeAnnotation = std::move(typeAnnotation),
    };
}

VnlcGenericParameterListParsingResult VnlcParser::parseGenericParameterList() {
    std::vector<std::string> parameters;

    do {
        if (!check(VnlcTokenType::IDENTIFIER)) {
            throw VnlcSyntaxError("Expected identifier in generic parameter list", peek().getLine(), peek().getColumn());
        } else {
            parameters.emplace_back(peek().getValue());
            advance();
        }
    } while (match(VnlcTokenType::COMMA));

    return VnlcGenericParameterListParsingResult{
        .parameters = std::move(parameters),
    };
}

VnlcGenericArgumentListParsingResult VnlcParser::parseGenericArgumentList() {
    std::vector<std::unique_ptr<VnlcTypeNode>> arguments;

    do {
        auto typeResult = parseType();
        arguments.push_back(std::move(typeResult.type));
    } while (match(VnlcTokenType::COMMA));

    return VnlcGenericArgumentListParsingResult{
        .arguments = std::move(arguments),
    };
}

VnlcAbsoluteImportPathListParsingResult VnlcParser::parseAbsoluteImportPathList() {
    std::vector<VnlcImportDeclarationItem> paths;

    do {
        auto itemResult = parseAbsoluteImportPathItem();
        paths.append_range(itemResult.paths);
    } while (match(VnlcTokenType::COMMA));

    return VnlcAbsoluteImportPathListParsingResult{
        .paths = std::move(paths),
    };
}

VnlcRelativeImportPathListParsingResult VnlcParser::parseRelativeImportPathList() {
    std::vector<VnlcImportDeclarationItem> paths;

    do {
        auto itemResult = parseRelativeImportPathItem();
        paths.append_range(itemResult.paths);
    } while (match(VnlcTokenType::COMMA));

    return VnlcRelativeImportPathListParsingResult{
        .paths = std::move(paths),
    };
}

VnlcFunctionBodyParsingResult VnlcParser::parseFunctionBody() {
    auto result = parseBlockStatement();

    return VnlcFunctionBodyParsingResult{
        .body = std::move(result.statement),
    };
}

VnlcClassBodyParsingResult VnlcParser::parseClassBody() {
    std::vector<std::unique_ptr<VnlcDeclarationNode>> declarations;

    if (!match(VnlcTokenType::LEFT_BRACE)) {
        throw VnlcSyntaxError("Expected '{' at the beginning of class body", peek().getLine(), peek().getColumn());
    }

    while (!check(VnlcTokenType::RIGHT_BRACE)) {
        auto result = parseClassMember();
        declarations.push_back(std::move(result.declaration));
    }

    if (!match(VnlcTokenType::RIGHT_BRACE)) {
        throw VnlcSyntaxError("Expected '}' at the end of class body", peek().getLine(), peek().getColumn());
    }

    return VnlcClassBodyParsingResult{
        .declarations = std::move(declarations),
    };
}

VnlcInterfaceBodyParsingResult VnlcParser::parseInterfaceBody() {
    std::vector<std::unique_ptr<VnlcFunctionDeclarationNode>> declarations;

    if (!match(VnlcTokenType::LEFT_BRACE)) {
        throw VnlcSyntaxError("Expected '{' at the beginning of interface body", peek().getLine(), peek().getColumn());
    }

    while (!check(VnlcTokenType::RIGHT_BRACE)) {
        auto result = parseInterfaceMethodDeclaration();
        declarations.push_back(std::move(result.declaration));
    }

    if (!match(VnlcTokenType::RIGHT_BRACE)) {
        throw VnlcSyntaxError("Expected '}' at the end of interface body", peek().getLine(), peek().getColumn());
    }

    return VnlcInterfaceBodyParsingResult{
        .declarations = std::move(declarations),
    };
}

VnlcEnumBodyParsingResult VnlcParser::parseEnumBody() {
    std::vector<std::unique_ptr<VnlcEnumMemberDeclarationNode>> declarations;

    if (!match(VnlcTokenType::LEFT_BRACE)) {
        throw VnlcSyntaxError("Expected '{' at the beginning of enum body", peek().getLine(), peek().getColumn());
    }

    while (!check(VnlcTokenType::RIGHT_BRACE)) {
        auto result = parseEnumMemberDeclaration();
        declarations.push_back(std::move(result.declaration));
    }

    if (!match(VnlcTokenType::RIGHT_BRACE)) {
        throw VnlcSyntaxError("Expected '}' at the end of enum body", peek().getLine(), peek().getColumn());
    }

    return VnlcEnumBodyParsingResult{
        .declarations = std::move(declarations),
    };
}

VnlcAbsoluteImportPathItemParsingResult VnlcParser::parseAbsoluteImportPathItem() {
    std::vector<std::string> nameParts;
    std::optional<std::string> alias = std::nullopt;

    if (match(VnlcTokenType::SELF)) {
        if (match(VnlcTokenType::AS)) {
            if (!check(VnlcTokenType::IDENTIFIER)) {
                throw VnlcSyntaxError("Expected identifier after 'as' keyword in import path", peek().getLine(), peek().getColumn());
            } else {
                alias = std::make_optional<std::string>(peek().getValue());
                advance();
            }
        }

        return VnlcAbsoluteImportPathItemParsingResult{
            .paths = { VnlcImportDeclarationItem{
                .nameParts = std::move(nameParts),
                .alias = std::move(alias),
            } },
        };
    } else {
        auto result = parseAbsoluteImportPath();

        return VnlcAbsoluteImportPathItemParsingResult{
            .paths = std::move(result.paths),
        };
    }
}

VnlcRelativeImportPathItemParsingResult VnlcParser::parseRelativeImportPathItem() {
    std::vector<std::string> nameParts;
    std::optional<std::string> alias = std::nullopt;

    if (match(VnlcTokenType::SELF)) {
        if (match(VnlcTokenType::AS)) {
            if (!check(VnlcTokenType::IDENTIFIER)) {
                throw VnlcSyntaxError("Expected identifier after 'as' keyword in import path", peek().getLine(), peek().getColumn());
            } else {
                alias = std::make_optional<std::string>(peek().getValue());
                advance();
            }
        }

        return VnlcRelativeImportPathItemParsingResult{
            .paths = { VnlcImportDeclarationItem{
                .nameParts = std::move(nameParts),
                .alias = std::move(alias),
            } },
        };
    } else {
        auto result = parseRelativeImportPath();

        return VnlcRelativeImportPathItemParsingResult{
            .paths = std::move(result.paths),
        };
    }
}

VnlcClassMemberParsingResult VnlcParser::parseClassMember() {
    VnlcToken firstToken = peekValid();

    bool hasMetadata = false;
    std::vector<VnlcDeclarationItem::MetadataTerm> metadataTerms;

    enum class AccessModifier {
        PUBLIC,
        PROTECTED,
        PRIVATE,
    };

    enum class Binding {
        INSTANCE,
        STATIC,
    };

    AccessModifier accessModifier = AccessModifier::PUBLIC;
    Binding binding = Binding::INSTANCE;

    if (check(VnlcTokenType::METADATA)) {
        hasMetadata = true;
        auto metadataResult = parseMetadata();
        metadataTerms = std::move(metadataResult.metadata);
    }

    if (match(VnlcTokenType::PUBLIC)) {
        accessModifier = AccessModifier::PUBLIC;
    } else if (match(VnlcTokenType::PRIVATE)) {
        accessModifier = AccessModifier::PRIVATE;
    }

    if (match(VnlcTokenType::STATIC)) {
        binding = Binding::STATIC;
    } else if (match(VnlcTokenType::OVERRIDE)) {
        VnlcFunctionDeclarationParsingContext functionDeclarationContext{
            .context = VnlcFunctionDeclarationType::Context::CLASS,
            .accessModifier = static_cast<VnlcFunctionDeclarationType::AccessModifier>(accessModifier),
            .binding = static_cast<VnlcFunctionDeclarationType::Binding>(binding),
            .hasMetadata = hasMetadata,
            .metadataTerms = std::move(metadataTerms),
        };
        auto functionDeclarationResult = parseFunctionDeclaration(functionDeclarationContext);

        VnlcToken lastToken = peek();
        functionDeclarationResult.declaration->resetPosition(firstToken, lastToken);

        if (!matchSeparatorEndOfLine()) {
            throw VnlcSyntaxError("Expected newline after method declaration", peek().getLine(), peek().getColumn());
        }

        return VnlcClassMemberParsingResult{
            .declaration = std::move(functionDeclarationResult.declaration),
        };
    }

    if (check(VnlcTokenType::FUNC) || check(VnlcTokenType::NATIVE)) {
        VnlcFunctionDeclarationParsingContext functionDeclarationContext{
            .context = VnlcFunctionDeclarationType::Context::CLASS,
            .accessModifier = static_cast<VnlcFunctionDeclarationType::AccessModifier>(accessModifier),
            .binding = static_cast<VnlcFunctionDeclarationType::Binding>(binding),
            .hasMetadata = hasMetadata,
            .metadataTerms = std::move(metadataTerms),
        };
        auto functionDeclarationResult = parseFunctionDeclaration(functionDeclarationContext);

        VnlcToken lastToken = peek();
        functionDeclarationResult.declaration->resetPosition(firstToken, lastToken);

        if (!matchSeparatorEndOfLine()) {
            throw VnlcSyntaxError("Expected newline after method declaration", peek().getLine(), peek().getColumn());
        }

        return VnlcClassMemberParsingResult{
            .declaration = std::move(functionDeclarationResult.declaration),
        };
    } else {
        VnlcPropertyDeclarationParsingContext propertyDeclarationContext{
            .accessModifier = static_cast<VnlcPropertyDeclarationType::AccessModifier>(accessModifier),
            .binding = static_cast<VnlcPropertyDeclarationType::Binding>(binding),
            .hasMetadata = hasMetadata,
            .metadataTerms = std::move(metadataTerms),
        };
        auto propertyDeclarationResult = parsePropertyDeclaration(propertyDeclarationContext);

        VnlcToken lastToken = peek();
        propertyDeclarationResult.declaration->resetPosition(firstToken, lastToken);

        if (!matchSeparatorEndOfLine()) {
            throw VnlcSyntaxError("Expected newline after property declaration", peek().getLine(), peek().getColumn());
        }

        return VnlcClassMemberParsingResult{
            .declaration = std::move(propertyDeclarationResult.declaration),
        };
    }
}

VnlcEnumMemberDeclarationParsingResult VnlcParser::parseEnumMemberDeclaration() {
    VnlcToken firstToken = peekValid();

    bool hasMetadata = false;
    std::vector<VnlcDeclarationItem::MetadataTerm> metadataTerms;
    std::string name;
    std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>> associatedValues;

    if (check(VnlcTokenType::METADATA)) {
        hasMetadata = true;
        auto metadataResult = parseMetadata();
        metadataTerms = std::move(metadataResult.metadata);
    }

    if (!check(VnlcTokenType::IDENTIFIER)) {
        throw VnlcSyntaxError("Expected enum member name", peek().getLine(), peek().getColumn());
    } else {
        name = peek().getValue();
        advance();
    }

    if (match(VnlcTokenType::LEFT_PARENTHESIS)) {
        if (!check(VnlcTokenType::RIGHT_PARENTHESIS)) {
            auto enumAssoicatedValueListResult = parseEnumAssociatedValueList();
            for (auto& associatedValue : enumAssoicatedValueListResult.associatedValues) {
                associatedValues.emplace_back(std::move(associatedValue.name), std::move(associatedValue.typeAnnotation));
            }
        }

        if (!match(VnlcTokenType::RIGHT_PARENTHESIS)) {
            throw VnlcSyntaxError("Expected ')' after enum member associated value list", peek().getLine(), peek().getColumn());
        }
    }

    VnlcToken lastToken = peek();

    if (!matchSeparatorEndOfLine()) {
        throw VnlcSyntaxError("Expected newline after enum member declaration", peek().getLine(), peek().getColumn());
    }

    if (hasMetadata) {
        return VnlcEnumMemberDeclarationParsingResult{
            .declaration = std::make_unique<VnlcEnumMemberDeclarationNode>(std::move(name), std::move(associatedValues), firstToken, lastToken, std::move(metadataTerms)),
        };
    } else {
        return VnlcEnumMemberDeclarationParsingResult{
            .declaration = std::make_unique<VnlcEnumMemberDeclarationNode>(std::move(name), std::move(associatedValues), firstToken, lastToken),
        };
    }
}

VnlcEnumAssociatedValueListParsingResult VnlcParser::parseEnumAssociatedValueList() {
    std::vector<VnlcEnumAssociatedValueListParsingResult::Item> items;

    do {
        auto result = parseEnumAssociatedValue();
        items.emplace_back(std::move(result.name), std::move(result.typeAnnotation));
    } while (match(VnlcTokenType::COMMA));

    return VnlcEnumAssociatedValueListParsingResult{
        .associatedValues = std::move(items),
    };
}

VnlcEnumAssociatedValueParsingResult VnlcParser::parseEnumAssociatedValue() {
    std::string name;
    std::unique_ptr<VnlcTypeAnnotationNode> typeAnnotation;

    if (!check(VnlcTokenType::IDENTIFIER)) {
        throw VnlcSyntaxError("Expected parameter name", peek().getLine(), peek().getColumn());
    } else {
        name = peek().getValue();
        advance();
    }

    if (!match(VnlcTokenType::COLON)) {
        throw VnlcSyntaxError("Expected ':' after parameter name", peek().getLine(), peek().getColumn());
    }

    auto typeAnnotationResult = parseTypeAnnotation();
    typeAnnotation = std::move(typeAnnotationResult.typeAnnotation);

    return VnlcEnumAssociatedValueParsingResult{
        .name = std::move(name),
        .typeAnnotation = std::move(typeAnnotation),
    };
}

VnlcExpressionParsingResult VnlcParser::parseExpression() {
    auto result = parseAssignmentExpression();

    return VnlcExpressionParsingResult{
        .expression = std::move(result.expression),
    };
}

VnlcAssignmentExpressionParsingResult VnlcParser::parseAssignmentExpression() {
    static const std::unordered_set<VnlcTokenType> assignmentOperators = {
        VnlcTokenType::EQUAL,
        VnlcTokenType::DOUBLE_QUESTION_EQUAL,
        VnlcTokenType::PLUS_EQUAL,
        VnlcTokenType::MINUS_EQUAL,
        VnlcTokenType::ASTERISK_EQUAL,
        VnlcTokenType::SLASH_EQUAL,
        VnlcTokenType::DOUBLE_SLASH_EQUAL,
        VnlcTokenType::PERCENT_EQUAL,
        VnlcTokenType::DOUBLE_ASTERISK_EQUAL,
        VnlcTokenType::AMPERSAND_EQUAL,
        VnlcTokenType::CARET_EQUAL,
        VnlcTokenType::PIPE_EQUAL,
        VnlcTokenType::DOUBLE_LEFT_ANGLE,
        VnlcTokenType::DOUBLE_RIGHT_ANGLE,
        VnlcTokenType::TRIPLE_RIGHT_ANGLE,
    };

    static const std::unordered_map<VnlcTokenType, VnlcBinaryExpressionType> assignmentExpressionTypes = {
        { VnlcTokenType::EQUAL, VnlcBinaryExpressionType::ASSIGNMENT },
        { VnlcTokenType::DOUBLE_QUESTION_EQUAL, VnlcBinaryExpressionType::NULLISH_COALESCING_ASSIGNMENT },
        { VnlcTokenType::PLUS_EQUAL, VnlcBinaryExpressionType::ADDITION_ASSIGNMENT },
        { VnlcTokenType::MINUS_EQUAL, VnlcBinaryExpressionType::SUBTRACTION_ASSIGNMENT },
        { VnlcTokenType::ASTERISK_EQUAL, VnlcBinaryExpressionType::MULTIPLICATION_ASSIGNMENT },
        { VnlcTokenType::SLASH_EQUAL, VnlcBinaryExpressionType::DIVISION_ASSIGNMENT },
        { VnlcTokenType::DOUBLE_SLASH_EQUAL, VnlcBinaryExpressionType::INTEGER_DIVISION_ASSIGNMENT },
        { VnlcTokenType::PERCENT_EQUAL, VnlcBinaryExpressionType::MODULO_ASSIGNMENT },
        { VnlcTokenType::DOUBLE_ASTERISK_EQUAL, VnlcBinaryExpressionType::EXPONENT_ASSIGNMENT },
        { VnlcTokenType::AMPERSAND_EQUAL, VnlcBinaryExpressionType::BITWISE_AND_ASSIGNMENT },
        { VnlcTokenType::CARET_EQUAL, VnlcBinaryExpressionType::BITWISE_XOR_ASSIGNMENT },
        { VnlcTokenType::PIPE_EQUAL, VnlcBinaryExpressionType::BITWISE_OR_ASSIGNMENT },
        { VnlcTokenType::DOUBLE_LEFT_ANGLE, VnlcBinaryExpressionType::SHIFT_LEFT_ASSIGNMENT },
        { VnlcTokenType::DOUBLE_RIGHT_ANGLE, VnlcBinaryExpressionType::SHIFT_RIGHT_ASSIGNMENT },
        { VnlcTokenType::TRIPLE_RIGHT_ANGLE, VnlcBinaryExpressionType::SHIFT_RIGHT_UNSIGNED_ASSIGNMENT },
    };

    VnlcToken firstToken = peekValid();

    auto leftResult = parseConditionalExpression();

    if (checkAny(assignmentOperators)) {
        VnlcBinaryExpressionType operatorType = assignmentExpressionTypes.at(peek().getType());
        advance();
        auto rightResult = parseAssignmentExpression();

        VnlcToken lastToken = peek();

        return VnlcAssignmentExpressionParsingResult{
            .expression = std::make_unique<VnlcBinaryExpressionNode>(operatorType, std::move(leftResult.expression), std::move(rightResult.expression), firstToken, lastToken),
        };
    } else {
        return VnlcAssignmentExpressionParsingResult{
            .expression = std::move(leftResult.expression),
        };
    }
}

VnlcConditionalExpressionParsingResult VnlcParser::parseConditionalExpression() {
    VnlcToken firstToken = peekValid();

    auto leftResult = parseNullishCoalescingExpression();

    if (match(VnlcTokenType::QUESTION)) {
        auto middleResult = parseAssignmentExpression();

        if (!match(VnlcTokenType::COLON)) {
            throw VnlcSyntaxError("Expected ':' in conditional expression", peek().getLine(), peek().getColumn());
        }

        auto rightResult = parseConditionalExpression();

        VnlcToken lastToken = peek();

        return VnlcConditionalExpressionParsingResult{
            .expression =
                std::make_unique<VnlcConditionalExpressionNode>(std::move(leftResult.expression), std::move(middleResult.expression), std::move(rightResult.expression), firstToken, lastToken),
        };
    } else {
        return VnlcConditionalExpressionParsingResult{
            .expression = std::move(leftResult.expression),
        };
    }
}

VnlcNullishCoalescingExpressionParsingResult VnlcParser::parseNullishCoalescingExpression() {
    VnlcToken firstToken = peekValid();

    auto leftResult = parseLogicalOrExpression();
    std::unique_ptr<VnlcExpressionNode> currentNode = std::move(leftResult.expression);

    while (match(VnlcTokenType::DOUBLE_QUESTION)) {
        auto rightResult = parseLogicalOrExpression();

        VnlcToken lastToken = peek();

        currentNode =
            std::make_unique<VnlcBinaryExpressionNode>(VnlcBinaryExpressionType::NULLISH_COALESCING, std::move(currentNode), std::move(rightResult.expression), firstToken, lastToken);
    }

    return VnlcNullishCoalescingExpressionParsingResult{
        .expression = std::move(currentNode),
    };
}

VnlcLogicalOrExpressionParsingResult VnlcParser::parseLogicalOrExpression() {
    VnlcToken firstToken = peekValid();

    auto leftResult = parseLogicalAndExpression();
    std::unique_ptr<VnlcExpressionNode> currentNode = std::move(leftResult.expression);

    while (match(VnlcTokenType::DOUBLE_PIPE)) {
        auto rightResult = parseLogicalAndExpression();

        VnlcToken lastToken = peek();

        currentNode = std::make_unique<VnlcBinaryExpressionNode>(VnlcBinaryExpressionType::LOGICAL_OR, std::move(currentNode), std::move(rightResult.expression), firstToken, lastToken);
    }

    return VnlcLogicalOrExpressionParsingResult{
        .expression = std::move(currentNode),
    };
}

VnlcLogicalAndExpressionParsingResult VnlcParser::parseLogicalAndExpression() {
    VnlcToken firstToken = peekValid();

    auto leftResult = parseEqualityExpression();
    std::unique_ptr<VnlcExpressionNode> currentNode = std::move(leftResult.expression);

    while (match(VnlcTokenType::DOUBLE_AMPERSAND)) {
        auto rightResult = parseEqualityExpression();

        VnlcToken lastToken = peek();

        currentNode = std::make_unique<VnlcBinaryExpressionNode>(VnlcBinaryExpressionType::LOGICAL_AND, std::move(currentNode), std::move(rightResult.expression), firstToken, lastToken);
    }

    return VnlcLogicalAndExpressionParsingResult{
        .expression = std::move(currentNode),
    };
}

VnlcEqualityExpressionParsingResult VnlcParser::parseEqualityExpression() {
    static const std::unordered_set<VnlcTokenType> equalityOperators = {
        VnlcTokenType::DOUBLE_EQUAL,
        VnlcTokenType::EXCLAMATION_EQUAL,
    };

    static const std::unordered_map<VnlcTokenType, VnlcBinaryExpressionType> equalityExpressionTypes = {
        { VnlcTokenType::DOUBLE_EQUAL, VnlcBinaryExpressionType::EQUAL },
        { VnlcTokenType::EXCLAMATION_EQUAL, VnlcBinaryExpressionType::NOT_EQUAL },
    };

    VnlcToken firstToken = peekValid();

    auto leftResult = parseRelationalExpression();
    std::unique_ptr<VnlcExpressionNode> currentNode = std::move(leftResult.expression);

    while (checkAny(equalityOperators)) {
        VnlcBinaryExpressionType operatorType = equalityExpressionTypes.at(peek().getType());
        advance();
        auto rightResult = parseRelationalExpression();

        VnlcToken lastToken = peek();

        currentNode = std::make_unique<VnlcBinaryExpressionNode>(operatorType, std::move(currentNode), std::move(rightResult.expression), firstToken, lastToken);
    }

    return VnlcEqualityExpressionParsingResult{
        .expression = std::move(currentNode),
    };
}

VnlcRelationalExpressionParsingResult VnlcParser::parseRelationalExpression() {
    static const std::unordered_set<VnlcTokenType> relationalOperators = {
        VnlcTokenType::LEFT_ANGLE,
        VnlcTokenType::LEFT_ANGLE_EQUAL,
        VnlcTokenType::RIGHT_ANGLE,
        VnlcTokenType::RIGHT_ANGLE_EQUAL,
    };

    static const std::unordered_map<VnlcTokenType, VnlcBinaryExpressionType> relationalExpressionTypes = {
        { VnlcTokenType::LEFT_ANGLE, VnlcBinaryExpressionType::LESS_THAN },
        { VnlcTokenType::LEFT_ANGLE_EQUAL, VnlcBinaryExpressionType::LESS_THAN_OR_EQUAL },
        { VnlcTokenType::RIGHT_ANGLE, VnlcBinaryExpressionType::GREATER_THAN },
        { VnlcTokenType::RIGHT_ANGLE_EQUAL, VnlcBinaryExpressionType::GREATER_THAN_OR_EQUAL },
    };

    VnlcToken firstToken = peekValid();

    auto leftResult = parseRangeExpression();
    std::unique_ptr<VnlcExpressionNode> currentNode = std::move(leftResult.expression);

    while (checkAny(relationalOperators)) {
        VnlcBinaryExpressionType operatorType = relationalExpressionTypes.at(peek().getType());
        advance();
        auto rightResult = parseRangeExpression();

        VnlcToken lastToken = peek();

        currentNode = std::make_unique<VnlcBinaryExpressionNode>(operatorType, std::move(currentNode), std::move(rightResult.expression), firstToken, lastToken);
    }

    return VnlcRelationalExpressionParsingResult{
        .expression = std::move(currentNode),
    };
}

VnlcRangeExpressionParsingResult VnlcParser::parseRangeExpression() {
    VnlcToken firstToken = peekValid();

    if (match(VnlcTokenType::DOUBLE_DOT)) {
        auto endResult = parseBitwiseOrExpression();
        VnlcToken lastToken = peek();

        return VnlcRangeExpressionParsingResult{
            .expression = std::make_unique<VnlcRangeExpressionNode>(std::nullopt, std::make_optional(std::move(endResult.expression)), firstToken, lastToken),
        };
    } else {
        auto startResult = parseBitwiseOrExpression();

        if (match(VnlcTokenType::DOUBLE_DOT)) {
            static const std::unordered_set<VnlcTokenType> rangeEndExpressionStarters = {
                VnlcTokenType::IDENTIFIER,
                VnlcTokenType::NUMBER,
                VnlcTokenType::STRING,
                VnlcTokenType::CHAR,
                VnlcTokenType::TRUE,
                VnlcTokenType::FALSE,
                VnlcTokenType::THIS,
                VnlcTokenType::SUPER,
                VnlcTokenType::LEFT_PARENTHESIS,
                VnlcTokenType::LEFT_BRACKET,
                VnlcTokenType::LEFT_BRACE,
                VnlcTokenType::PLUS,
                VnlcTokenType::MINUS,
                VnlcTokenType::TILDE,
                VnlcTokenType::EXCLAMATION,
                VnlcTokenType::SELECTOR_PREFIX,
            };

            if (checkAny(rangeEndExpressionStarters)) {
                auto endResult = parseBitwiseOrExpression();
                VnlcToken lastToken = peek();

                return VnlcRangeExpressionParsingResult{
                    .expression = std::make_unique<VnlcRangeExpressionNode>(
                        std::make_optional(std::move(startResult.expression)),
                        std::make_optional(std::move(endResult.expression)),
                        firstToken,
                        lastToken
                    ),
                };
            } else {
                VnlcToken lastToken = peek();

                return VnlcRangeExpressionParsingResult{
                    .expression = std::make_unique<VnlcRangeExpressionNode>(std::make_optional(std::move(startResult.expression)), std::nullopt, firstToken, lastToken),
                };
            }
        } else {
            return VnlcRangeExpressionParsingResult{
                .expression = std::move(startResult.expression),
            };
        }
    }
}

VnlcBitwiseOrExpressionParsingResult VnlcParser::parseBitwiseOrExpression() {
    VnlcToken firstToken = peekValid();

    auto leftResult = parseBitwiseXorExpression();
    std::unique_ptr<VnlcExpressionNode> currentNode = std::move(leftResult.expression);

    while (match(VnlcTokenType::PIPE)) {
        auto rightResult = parseBitwiseXorExpression();

        VnlcToken lastToken = peek();

        currentNode = std::make_unique<VnlcBinaryExpressionNode>(VnlcBinaryExpressionType::BITWISE_OR, std::move(currentNode), std::move(rightResult.expression), firstToken, lastToken);
    }

    return VnlcBitwiseOrExpressionParsingResult{
        .expression = std::move(currentNode),
    };
}

VnlcBitwiseXorExpressionParsingResult VnlcParser::parseBitwiseXorExpression() {
    VnlcToken firstToken = peekValid();

    auto leftResult = parseBitwiseAndExpression();
    std::unique_ptr<VnlcExpressionNode> currentNode = std::move(leftResult.expression);

    while (match(VnlcTokenType::CARET)) {
        auto rightResult = parseBitwiseAndExpression();

        VnlcToken lastToken = peek();

        currentNode = std::make_unique<VnlcBinaryExpressionNode>(VnlcBinaryExpressionType::BITWISE_XOR, std::move(currentNode), std::move(rightResult.expression), firstToken, lastToken);
    }

    return VnlcBitwiseXorExpressionParsingResult{
        .expression = std::move(currentNode),
    };
}

VnlcBitwiseAndExpressionParsingResult VnlcParser::parseBitwiseAndExpression() {
    VnlcToken firstToken = peekValid();

    auto leftResult = parseShiftExpression();
    std::unique_ptr<VnlcExpressionNode> currentNode = std::move(leftResult.expression);

    while (match(VnlcTokenType::AMPERSAND)) {
        auto rightResult = parseShiftExpression();

        VnlcToken lastToken = peek();

        currentNode = std::make_unique<VnlcBinaryExpressionNode>(VnlcBinaryExpressionType::BITWISE_AND, std::move(currentNode), std::move(rightResult.expression), firstToken, lastToken);
    }

    return VnlcBitwiseAndExpressionParsingResult{
        .expression = std::move(currentNode),
    };
}

VnlcShiftExpressionParsingResult VnlcParser::parseShiftExpression() {
    static const std::unordered_set<VnlcTokenType> shiftOperators = {
        VnlcTokenType::DOUBLE_LEFT_ANGLE,
        VnlcTokenType::DOUBLE_RIGHT_ANGLE,
        VnlcTokenType::TRIPLE_RIGHT_ANGLE,
    };

    static const std::unordered_map<VnlcTokenType, VnlcBinaryExpressionType> shiftExpressionTypes = {
        { VnlcTokenType::DOUBLE_LEFT_ANGLE, VnlcBinaryExpressionType::SHIFT_LEFT },
        { VnlcTokenType::DOUBLE_RIGHT_ANGLE, VnlcBinaryExpressionType::SHIFT_RIGHT },
        { VnlcTokenType::TRIPLE_RIGHT_ANGLE, VnlcBinaryExpressionType::SHIFT_RIGHT_UNSIGNED },
    };

    VnlcToken firstToken = peekValid();

    auto leftResult = parseAdditiveExpression();
    std::unique_ptr<VnlcExpressionNode> currentNode = std::move(leftResult.expression);

    while (checkAny(shiftOperators)) {
        VnlcBinaryExpressionType operatorType = shiftExpressionTypes.at(peek().getType());
        advance();
        auto rightResult = parseAdditiveExpression();

        VnlcToken lastToken = peek();

        currentNode = std::make_unique<VnlcBinaryExpressionNode>(operatorType, std::move(currentNode), std::move(rightResult.expression), firstToken, lastToken);
    }

    return VnlcShiftExpressionParsingResult{
        .expression = std::move(currentNode),
    };
}

VnlcAdditiveExpressionParsingResult VnlcParser::parseAdditiveExpression() {
    static const std::unordered_set<VnlcTokenType> additiveOperators = {
        VnlcTokenType::PLUS,
        VnlcTokenType::MINUS,
    };

    static const std::unordered_map<VnlcTokenType, VnlcBinaryExpressionType> additiveExpressionTypes = {
        { VnlcTokenType::PLUS, VnlcBinaryExpressionType::ADDITION },
        { VnlcTokenType::MINUS, VnlcBinaryExpressionType::SUBTRACTION },
    };

    VnlcToken firstToken = peekValid();

    auto leftResult = parseMultiplicativeExpression();
    std::unique_ptr<VnlcExpressionNode> currentNode = std::move(leftResult.expression);

    while (checkAny(additiveOperators)) {
        VnlcBinaryExpressionType operatorType = additiveExpressionTypes.at(peek().getType());
        advance();
        auto rightResult = parseMultiplicativeExpression();

        VnlcToken lastToken = peek();

        currentNode = std::make_unique<VnlcBinaryExpressionNode>(operatorType, std::move(currentNode), std::move(rightResult.expression), firstToken, lastToken);
    }

    return VnlcAdditiveExpressionParsingResult{
        .expression = std::move(currentNode),
    };
}

VnlcMultiplicativeExpressionParsingResult VnlcParser::parseMultiplicativeExpression() {
    static const std::unordered_set<VnlcTokenType> multiplicativeOperators = {
        VnlcTokenType::ASTERISK,
        VnlcTokenType::SLASH,
        VnlcTokenType::DOUBLE_SLASH,
        VnlcTokenType::PERCENT,
    };

    static const std::unordered_map<VnlcTokenType, VnlcBinaryExpressionType> multiplicativeExpressionTypes = {
        { VnlcTokenType::ASTERISK, VnlcBinaryExpressionType::MULTIPLICATION },
        { VnlcTokenType::SLASH, VnlcBinaryExpressionType::DIVISION },
        { VnlcTokenType::DOUBLE_SLASH, VnlcBinaryExpressionType::INTEGER_DIVISION },
        { VnlcTokenType::PERCENT, VnlcBinaryExpressionType::MODULO },
    };

    VnlcToken firstToken = peekValid();

    auto leftResult = parseUnaryExpression();
    std::unique_ptr<VnlcExpressionNode> currentNode = std::move(leftResult.expression);

    while (checkAny(multiplicativeOperators)) {
        VnlcBinaryExpressionType operatorType = multiplicativeExpressionTypes.at(peek().getType());
        advance();
        auto rightResult = parseUnaryExpression();

        VnlcToken lastToken = peek();

        currentNode = std::make_unique<VnlcBinaryExpressionNode>(operatorType, std::move(currentNode), std::move(rightResult.expression), firstToken, lastToken);
    }

    return VnlcMultiplicativeExpressionParsingResult{
        .expression = std::move(currentNode),
    };
}

VnlcUnaryExpressionParsingResult VnlcParser::parseUnaryExpression() {
    static const std::unordered_set<VnlcTokenType> unaryOperators = {
        VnlcTokenType::PLUS,
        VnlcTokenType::MINUS,
        VnlcTokenType::TILDE,
        VnlcTokenType::EXCLAMATION,
    };

    static const std::unordered_map<VnlcTokenType, VnlcUnaryExpressionType> unaryExpressionTypes = {
        { VnlcTokenType::PLUS, VnlcUnaryExpressionType::UNARY_PLUS },
        { VnlcTokenType::MINUS, VnlcUnaryExpressionType::UNARY_MINUS },
        { VnlcTokenType::TILDE, VnlcUnaryExpressionType::BITWISE_NOT },
        { VnlcTokenType::EXCLAMATION, VnlcUnaryExpressionType::LOGICAL_NOT },
    };

    VnlcToken firstToken = peekValid();

    if (checkAny(unaryOperators)) {
        VnlcUnaryExpressionType operatorType = unaryExpressionTypes.at(peek().getType());
        advance();
        auto operandResult = parseExponentialExpression();

        VnlcToken lastToken = peek();

        return VnlcUnaryExpressionParsingResult{
            .expression = std::make_unique<VnlcUnaryExpressionNode>(operatorType, std::move(operandResult.expression), firstToken, lastToken),
        };
    } else {
        auto result = parseExponentialExpression();

        return VnlcUnaryExpressionParsingResult{
            .expression = std::move(result.expression),
        };
    }
}

VnlcExponentialExpressionParsingResult VnlcParser::parseExponentialExpression() {
    VnlcToken firstToken = peekValid();

    auto leftResult = parsePostfixExpression();

    if (match(VnlcTokenType::DOUBLE_ASTERISK)) {
        auto rightResult = parseUnaryExpression();

        VnlcToken lastToken = peek();

        return VnlcExponentialExpressionParsingResult{
            .expression =
                std::make_unique<VnlcBinaryExpressionNode>(VnlcBinaryExpressionType::EXPONENT, std::move(leftResult.expression), std::move(rightResult.expression), firstToken, lastToken),
        };
    } else {
        return VnlcExponentialExpressionParsingResult{
            .expression = std::move(leftResult.expression),
        };
    }
}

VnlcPostfixExpressionParsingResult VnlcParser::parsePostfixExpression() {
    VnlcToken firstToken = peekValid();

    auto primaryResult = parsePrimaryExpression();
    std::unique_ptr<VnlcExpressionNode> currentNode = std::move(primaryResult.expression);

    static const std::unordered_set<VnlcTokenType> postfixOperators = {
        VnlcTokenType::DOT,
        VnlcTokenType::QUESTION_DOT,
        VnlcTokenType::LEFT_PARENTHESIS,
        VnlcTokenType::LEFT_BRACKET,
    };

    while (checkAny(postfixOperators)) {
        if (match(VnlcTokenType::DOT)) {
            VnlcToken identifierFirstToken = peek();

            std::string name;
            if (!check(VnlcTokenType::IDENTIFIER)) {
                throw VnlcSyntaxError("Expected identifier after '.'", peek().getLine(), peek().getColumn());
            } else {
                name = peek().getValue();
                advance();
            }

            VnlcToken lastToken = peek();

            std::unique_ptr<VnlcIdentifierExpressionNode> nameNode = std::make_unique<VnlcIdentifierExpressionNode>(std::move(name), identifierFirstToken, lastToken);

            currentNode = std::make_unique<VnlcMemberAccessExpressionNode>(VnlcMemberAccessExpressionType::DOT, std::move(currentNode), std::move(nameNode), firstToken, lastToken);
        } else if (match(VnlcTokenType::QUESTION_DOT)) {
            VnlcToken identifierFirstToken = peek();

            std::string name;
            if (!check(VnlcTokenType::IDENTIFIER)) {
                throw VnlcSyntaxError("Expected identifier after '?.'", peek().getLine(), peek().getColumn());
            } else {
                name = peek().getValue();
                advance();
            }

            VnlcToken lastToken = peek();

            std::unique_ptr<VnlcIdentifierExpressionNode> nameNode = std::make_unique<VnlcIdentifierExpressionNode>(std::move(name), identifierFirstToken, lastToken);

            currentNode =
                std::make_unique<VnlcMemberAccessExpressionNode>(VnlcMemberAccessExpressionType::OPTIONAL_CHAINING, std::move(currentNode), std::move(nameNode), firstToken, lastToken);
        } else if (match(VnlcTokenType::LEFT_PARENTHESIS)) {
            std::vector<std::unique_ptr<VnlcExpressionNode>> arguments;

            if (!check(VnlcTokenType::RIGHT_PARENTHESIS)) {
                auto argumentListResult = parseArgumentList();
                arguments = std::move(argumentListResult.arguments);
            }

            if (!match(VnlcTokenType::RIGHT_PARENTHESIS)) {
                throw VnlcSyntaxError("Expected ')' after argument list", peek().getLine(), peek().getColumn());
            }

            VnlcToken lastToken = peek();

            currentNode = std::make_unique<VnlcFunctionCallExpressionNode>(std::move(currentNode), std::move(arguments), firstToken, lastToken);
        } else if (match(VnlcTokenType::LEFT_BRACKET)) {
            auto indexResult = parseExpression();

            if (!match(VnlcTokenType::RIGHT_BRACKET)) {
                throw VnlcSyntaxError("Expected ']' after index expression", peek().getLine(), peek().getColumn());
            }

            VnlcToken lastToken = peek();

            currentNode = std::make_unique<VnlcSubscriptExpressionNode>(std::move(currentNode), std::move(indexResult.expression), firstToken, lastToken);
        }
    }

    return VnlcPostfixExpressionParsingResult{
        .expression = std::move(currentNode),
    };
}

VnlcPrimaryExpressionParsingResult VnlcParser::parsePrimaryExpression() {
    static const std::unordered_set<VnlcTokenType> literalStarters = {
        VnlcTokenType::NUMBER, VnlcTokenType::STRING,       VnlcTokenType::CHAR,       VnlcTokenType::TRUE,
        VnlcTokenType::FALSE,  VnlcTokenType::LEFT_BRACKET, VnlcTokenType::LEFT_BRACE, VnlcTokenType::SELECTOR_PREFIX,
    };

    VnlcToken firstToken = peekValid();

    if (match(VnlcTokenType::LEFT_PARENTHESIS)) {
        auto expressionResult = parseExpression();

        if (!match(VnlcTokenType::RIGHT_PARENTHESIS)) {
            throw VnlcSyntaxError("Expected ')' after expression", peek().getLine(), peek().getColumn());
        }

        VnlcToken lastToken = peek();
        expressionResult.expression->resetPosition(firstToken, lastToken);

        return VnlcPrimaryExpressionParsingResult{
            .expression = std::move(expressionResult.expression),
        };
    } else if (match(VnlcTokenType::THIS)) {
        VnlcToken lastToken = peek();

        return VnlcPrimaryExpressionParsingResult{
            .expression = std::make_unique<VnlcThisExpressionNode>(firstToken, lastToken),
        };
    } else if (match(VnlcTokenType::SUPER)) {
        VnlcToken lastToken = peek();

        return VnlcPrimaryExpressionParsingResult{
            .expression = std::make_unique<VnlcSuperExpressionNode>(firstToken, lastToken),
        };
    } else if (check(VnlcTokenType::IDENTIFIER)) {
        std::string name(peek().getValue());
        advance();

        VnlcToken lastToken = peek();

        return VnlcPrimaryExpressionParsingResult{
            .expression = std::make_unique<VnlcIdentifierExpressionNode>(std::move(name), firstToken, lastToken),
        };
    } else if (checkAny(literalStarters)) {
        auto literalResult = parseLiteral();

        return VnlcPrimaryExpressionParsingResult{
            .expression = std::move(literalResult.expression),
        };
    } else {
        throw VnlcSyntaxError("Expected primary expression", peek().getLine(), peek().getColumn());
    }
}

VnlcLiteralParsingResult VnlcParser::parseLiteral() {
    VnlcToken firstToken = peekValid();

    if (match(VnlcTokenType::NUMBER)) {
        VnlcToken lastToken = peek();

        VnlcSimpleLiteralExpressionType literalType = VnlcSimpleLiteralExpressionType::DECIMAL_INTEGER;

        if (firstToken.getValue().find('.') != std::string::npos || firstToken.getValue().find('e') != std::string::npos || firstToken.getValue().find('E') != std::string::npos) {
            if (firstToken.getValue().ends_with('f') || firstToken.getValue().ends_with('F')) {
                literalType = VnlcSimpleLiteralExpressionType::DECIMAL_FLOAT;
            } else {
                literalType = VnlcSimpleLiteralExpressionType::DECIMAL_DOUBLE;
            }
        } else if (firstToken.getValue().starts_with("0x")) {
            literalType = VnlcSimpleLiteralExpressionType::HEXADECIMAL;
        } else if (firstToken.getValue().starts_with("0b")) {
            literalType = VnlcSimpleLiteralExpressionType::BINARY;
        } else if (firstToken.getValue().starts_with("0o")) {
            literalType = VnlcSimpleLiteralExpressionType::OCTAL;
        } else if (firstToken.getValue().ends_with(('b')) || firstToken.getValue().ends_with(('B'))) {
            literalType = VnlcSimpleLiteralExpressionType::DECIMAL_BYTE;
        } else if (firstToken.getValue().ends_with(('s')) || firstToken.getValue().ends_with(('S'))) {
            literalType = VnlcSimpleLiteralExpressionType::DECIMAL_SHORT;
        } else if (firstToken.getValue().ends_with(('l')) || firstToken.getValue().ends_with(('L'))) {
            literalType = VnlcSimpleLiteralExpressionType::DECIMAL_LONG;
        }

        return VnlcLiteralParsingResult{
            .expression = std::make_unique<VnlcSimpleLiteralExpressionNode>(literalType, firstToken.getValue(), firstToken, lastToken),
        };
    } else if (match(VnlcTokenType::CHAR)) {
        VnlcToken lastToken = peek();

        return VnlcLiteralParsingResult{
            .expression = std::make_unique<VnlcSimpleLiteralExpressionNode>(
                VnlcSimpleLiteralExpressionType::CHARACTER,
                firstToken.getValue().substr(1, firstToken.getValue().length() - 2), // Remove the surrounding single quotes
                firstToken,
                lastToken
            ),
        };
    } else if (check(VnlcTokenType::STRING)) {
        auto result = parseString();

        return VnlcLiteralParsingResult{
            .expression = std::move(result.expression),
        };
    } else if (check(VnlcTokenType::TRUE) || check(VnlcTokenType::FALSE)) {
        auto result = parseBoolean();

        return VnlcLiteralParsingResult{
            .expression = std::move(result.expression),
        };
    } else if (check(VnlcTokenType::LEFT_BRACKET)) {
        auto result = parseListLikeLiteral();

        return VnlcLiteralParsingResult{
            .expression = std::move(result.expression),
        };
    } else if (check(VnlcTokenType::LEFT_BRACE)) {
        auto result = parseDictLiteral();

        return VnlcLiteralParsingResult{
            .expression = std::move(result.expression),
        };
    } else if (check(VnlcTokenType::SELECTOR_PREFIX)) {
        auto result = parseSelector();

        return VnlcLiteralParsingResult{
            .expression = std::move(result.expression),
        };
    } else {
        throw VnlcSyntaxError("Expected literal", peek().getLine(), peek().getColumn());
    }
}

VnlcStringParsingResult VnlcParser::parseString() {
    VnlcToken firstToken = peekValid();
    std::vector<std::variant<std::string, std::unique_ptr<VnlcExpressionNode>>> parts;

    if (!check(VnlcTokenType::STRING)) {
        throw VnlcSyntaxError("Expected string literal", peek().getLine(), peek().getColumn());
    }

    if (peek().getValue().starts_with("\"")) {
        std::string value(peek().getValue().substr(1, peek().getValue().length() - 2));
        parts.emplace_back(std::move(value));
        advance();

        VnlcToken lastToken = peek();

        return VnlcStringParsingResult{
            .expression = std::make_unique<VnlcStringLiteralExpressionNode>(VnlcStringLiteralExpressionType::STRING, std::move(parts), firstToken, lastToken),
        };
    } else if (peek().getValue().starts_with("f\"")) {
        std::string value(peek().getValue().substr(2));
        if (value.ends_with('\"') && !value.ends_with("\\\"")) {
            value.pop_back();
        }
        parts.emplace_back(std::move(value));
        advance();

        while (check(VnlcTokenType::INTERPOLATION_START) || check(VnlcTokenType::STRING)) {
            if (check(VnlcTokenType::STRING)) {
                std::string value(peek().getValue());
                if (value.ends_with('\"') && !value.ends_with("\\\"")) {
                    value.pop_back();
                }
                parts.emplace_back(std::move(value));
                advance();
            } else if (check(VnlcTokenType::INTERPOLATION_START)) {
                auto interpolationResult = parseInterpolation();
                parts.emplace_back(std::move(interpolationResult.expression));
            } else {
                throw VnlcSyntaxError("Expected string literal or interpolation in formatted string", peek().getLine(), peek().getColumn());
            }
        }

        VnlcToken lastToken = peek();

        return VnlcStringParsingResult{
            .expression = std::make_unique<VnlcStringLiteralExpressionNode>(VnlcStringLiteralExpressionType::FORMAT_STRING, std::move(parts), firstToken, lastToken),
        };

    } else if (peek().getValue().starts_with("r\"")) {
        std::string value(peek().getValue().substr(2, peek().getValue().length() - 3)); // Remove the r" prefix and the surrounding quotes
        parts.emplace_back(std::move(value));
        advance();

        VnlcToken lastToken = peek();

        return VnlcStringParsingResult{
            .expression = std::make_unique<VnlcStringLiteralExpressionNode>(VnlcStringLiteralExpressionType::RAW_STRING, std::move(parts), firstToken, lastToken),
        };
    } else {
        throw VnlcSyntaxError("Invalid string literal", peek().getLine(), peek().getColumn());
    }
}

VnlcBooleanParsingResult VnlcParser::parseBoolean() {
    VnlcToken firstToken = peekValid();

    if (match(VnlcTokenType::TRUE) || match(VnlcTokenType::FALSE)) {
        bool value = firstToken.getType() == VnlcTokenType::TRUE;

        VnlcToken lastToken = peek();

        return VnlcBooleanParsingResult{
            .expression = std::make_unique<VnlcSimpleLiteralExpressionNode>(VnlcSimpleLiteralExpressionType::BOOLEAN, value ? "true" : "false", firstToken, lastToken),
        };
    } else {
        throw VnlcSyntaxError("Expected boolean literal", peek().getLine(), peek().getColumn());
    }
}

VnlcListLikeLiteralParsingResult VnlcParser::parseListLikeLiteral() {
    VnlcToken firstToken = peekValid();

    VnlcListLikeLiteralExpressionType literalType = VnlcListLikeLiteralExpressionType::LIST;

    if (!match(VnlcTokenType::LEFT_BRACKET)) {
        throw VnlcSyntaxError("Expected '[' to start list literal", peek().getLine(), peek().getColumn());
    }

    std::vector<std::unique_ptr<VnlcExpressionNode>> elements;

    if (check(VnlcTokenType::IDENTIFIER)) {
        std::string name(peek().getValue());
        advance();

        if ((name == "B" || name == "I" || name == "L")) {
            if (!match(VnlcTokenType::SEMICOLON)) {
                elements.push_back(std::make_unique<VnlcIdentifierExpressionNode>(std::move(name), firstToken, peek()));
                bool _ = match(VnlcTokenType::COMMA);
            } else {
                if (name == "B") {
                    literalType = VnlcListLikeLiteralExpressionType::BYTE_SNBT_ARRAY;
                } else if (name == "I") {
                    literalType = VnlcListLikeLiteralExpressionType::INT_SNBT_ARRAY;
                } else if (name == "L") {
                    literalType = VnlcListLikeLiteralExpressionType::LONG_SNBT_ARRAY;
                }
            }
        } else {
            elements.push_back(std::make_unique<VnlcIdentifierExpressionNode>(std::move(name), firstToken, peek()));
            bool _ = match(VnlcTokenType::COMMA);
        }
    }

    if (!check(VnlcTokenType::RIGHT_BRACKET)) {
        do {
            auto expressionResult = parseExpression();
            elements.push_back(std::move(expressionResult.expression));
        } while (match(VnlcTokenType::COMMA));
    }

    if (!match(VnlcTokenType::RIGHT_BRACKET)) {
        throw VnlcSyntaxError("Expected ']' to end list literal", peek().getLine(), peek().getColumn());
    }

    VnlcToken lastToken = peek();

    return VnlcListLikeLiteralParsingResult{
        .expression = std::make_unique<VnlcListLikeLiteralExpressionNode>(literalType, std::move(elements), firstToken, lastToken),
    };
}

VnlcDictLiteralParsingResult VnlcParser::parseDictLiteral() {
    VnlcToken firstToken = peekValid();

    if (!match(VnlcTokenType::LEFT_BRACE)) {
        throw VnlcSyntaxError("Expected '{' to start dict literal", peek().getLine(), peek().getColumn());
    }

    std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>> entries;

    if (!check(VnlcTokenType::RIGHT_BRACE)) {
        do {
            auto entryResult = parseDictEntry();
            entries.emplace(std::move(entryResult.key), std::move(entryResult.value));
        } while (match(VnlcTokenType::COMMA));
    }

    if (!match(VnlcTokenType::RIGHT_BRACE)) {
        throw VnlcSyntaxError("Expected '}' to end dict literal", peek().getLine(), peek().getColumn());
    }

    VnlcToken lastToken = peek();

    return VnlcDictLiteralParsingResult{
        .expression = std::make_unique<VnlcDictLiteralExpressionNode>(std::move(entries), firstToken, lastToken),
    };
}

VnlcSelectorParsingResult VnlcParser::parseSelector() {
    VnlcToken firstToken = peekValid();

    VnlcSelectorLiteralExpressionType literalType = VnlcSelectorLiteralExpressionType::NEAREST_PLAYER;
    std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>> arguments;

    if (!check(VnlcTokenType::SELECTOR_PREFIX)) {
        throw VnlcSyntaxError("Expected '@' to start selector", peek().getLine(), peek().getColumn());
    }

    if (peek().getValue() == "@p") {
        literalType = VnlcSelectorLiteralExpressionType::NEAREST_PLAYER;
    } else if (peek().getValue() == "@r") {
        literalType = VnlcSelectorLiteralExpressionType::RANDOM_PLAYER;
    } else if (peek().getValue() == "@a") {
        literalType = VnlcSelectorLiteralExpressionType::ALL_PLAYERS;
    } else if (peek().getValue() == "@e") {
        literalType = VnlcSelectorLiteralExpressionType::ALL_ENTITIES;
    } else if (peek().getValue() == "@s") {
        literalType = VnlcSelectorLiteralExpressionType::CURRENT_EXECUTOR;
    } else if (peek().getValue() == "@n") {
        literalType = VnlcSelectorLiteralExpressionType::NEAREST_ENTITY;
    } else {
        throw VnlcSyntaxError("Invalid selector type", peek().getLine(), peek().getColumn());
    }

    advance();

    if (match(VnlcTokenType::LEFT_BRACKET)) {
        if (!check(VnlcTokenType::RIGHT_BRACKET)) {
            auto argumentListResult = parseSelectorArgumentList();
            arguments = std::move(argumentListResult.arguments);
        }

        if (!match(VnlcTokenType::RIGHT_BRACKET)) {
            throw VnlcSyntaxError("Expected ']' to end selector arguments", peek().getLine(), peek().getColumn());
        }
    }

    VnlcToken lastToken = peek();

    return VnlcSelectorParsingResult{
        .expression = std::make_unique<VnlcSelectorLiteralExpressionNode>(literalType, std::move(arguments), firstToken, lastToken),
    };
}

VnlcArgumentListParsingResult VnlcParser::parseArgumentList() {
    std::vector<std::unique_ptr<VnlcExpressionNode>> arguments;

    do {
        auto expressionResult = parseExpression();
        arguments.push_back(std::move(expressionResult.expression));
    } while (match(VnlcTokenType::COMMA));

    return VnlcArgumentListParsingResult{
        .arguments = std::move(arguments),
    };
}

VnlcInterpolationParsingResult VnlcParser::parseInterpolation() {
    if (!match(VnlcTokenType::INTERPOLATION_START)) {
        throw VnlcSyntaxError("Expected '$(' to start interpolation", peek().getLine(), peek().getColumn());
    }

    auto expressionResult = parseExpression();

    if (!match(VnlcTokenType::RIGHT_PARENTHESIS)) {
        throw VnlcSyntaxError("Expected ')' to end interpolation", peek().getLine(), peek().getColumn());
    }

    return VnlcInterpolationParsingResult{
        .expression = std::move(expressionResult.expression),
    };
}

VnlcDictEntryParsingResult VnlcParser::parseDictEntry() {
    std::string key;
    if (check(VnlcTokenType::STRING)) {
        if (!peek().getValue().starts_with("\"")) {
            throw VnlcSyntaxError("Expected simple string literal for dict key", peek().getLine(), peek().getColumn());
        }

        key = peek().getValue().substr(1, peek().getValue().length() - 2); // Remove the surrounding quotes
        advance();
    } else if (checkGeneralizedIdentifier()) {
        key = peek().getValue();
        advance();
    } else {
        throw VnlcSyntaxError("Expected string literal or identifier for dict key", peek().getLine(), peek().getColumn());
    }

    if (!match(VnlcTokenType::COLON)) {
        throw VnlcSyntaxError("Expected ':' after dict key", peek().getLine(), peek().getColumn());
    }

    auto valueResult = parseExpression();

    return VnlcDictEntryParsingResult{
        .key = std::move(key),
        .value = std::move(valueResult.expression),
    };
}

VnlcSelectorArgumentListParsingResult VnlcParser::parseSelectorArgumentList() {
    std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>> arguments;

    do {
        auto argumentResult = parseSelectorArgument();
        arguments.emplace(std::move(argumentResult.key), std::move(argumentResult.value));
    } while (match(VnlcTokenType::COMMA));

    return VnlcSelectorArgumentListParsingResult{
        .arguments = std::move(arguments),
    };
}

VnlcSelectorArgumentParsingResult VnlcParser::parseSelectorArgument() {
    std::string key;

    if (!checkGeneralizedIdentifier()) {
        throw VnlcSyntaxError("Expected identifier for selector argument key", peek().getLine(), peek().getColumn());
    } else {
        key = peek().getValue();
        advance();
    }

    if (!match(VnlcTokenType::EQUAL)) {
        throw VnlcSyntaxError("Expected '=' after selector argument key", peek().getLine(), peek().getColumn());
    }

    auto valueResult = parseExpression();

    return VnlcSelectorArgumentParsingResult{
        .key = std::move(key),
        .value = std::move(valueResult.expression),
    };
}

VnlcStatementParsingResult VnlcParser::parseStatement() {
    static const std::unordered_set<VnlcTokenType> expressionStarters = {
        VnlcTokenType::IDENTIFIER,  VnlcTokenType::NUMBER,           VnlcTokenType::STRING,       VnlcTokenType::CHAR,       VnlcTokenType::TRUE, VnlcTokenType::FALSE, VnlcTokenType::THIS,
        VnlcTokenType::SUPER,       VnlcTokenType::LEFT_PARENTHESIS, VnlcTokenType::LEFT_BRACKET, VnlcTokenType::LEFT_BRACE, VnlcTokenType::PLUS, VnlcTokenType::MINUS, VnlcTokenType::TILDE,
        VnlcTokenType::EXCLAMATION, VnlcTokenType::SELECTOR_PREFIX,
    };

    static const std::unordered_set<VnlcTokenType> variableDeclarationStarters = {
        VnlcTokenType::VAR,
        VnlcTokenType::LET,
        VnlcTokenType::CONST,
    };

    static const std::unordered_set<VnlcTokenType> controlFlowStarters = {
        VnlcTokenType::IF,     VnlcTokenType::SWITCH, VnlcTokenType::LABEL,    VnlcTokenType::WHILE,  VnlcTokenType::FOR,
        VnlcTokenType::RETURN, VnlcTokenType::BREAK,  VnlcTokenType::CONTINUE, VnlcTokenType::RELOAD,
    };

    std::unique_ptr<VnlcStatementNode> statement;

    if (checkAny(expressionStarters)) {
        auto result = parseExpressionStatement();

        statement = std::move(result.statement);
    } else if (checkAny(variableDeclarationStarters)) {
        auto result = parseVariableDeclarationStatement();

        statement = std::move(result.statement);
    } else if (check(VnlcTokenType::LEFT_BRACE)) {
        auto result = parseBlockStatement();

        statement = std::move(result.statement);
    } else if (checkAny(controlFlowStarters)) {
        auto result = parseControlFlowStatement();

        statement = std::move(result.statement);
    } else {
        throw VnlcSyntaxError("Expected statement", peek().getLine(), peek().getColumn());
    }

    if (!matchSeparatorEndOfLine()) {
        throw VnlcSyntaxError("Expected newline after statement", peek().getLine(), peek().getColumn());
    }

    return VnlcStatementParsingResult{
        .statement = std::move(statement),
    };
}

VnlcExpressionStatementParsingResult VnlcParser::parseExpressionStatement() {
    VnlcToken firstToken = peekValid();

    auto result = parseExpression();

    VnlcToken lastToken = peek();

    return VnlcExpressionStatementParsingResult{
        .statement = std::make_unique<VnlcExpressionStatementNode>(std::move(result.expression), firstToken, lastToken),
    };
}

VnlcVariableDeclarationStatementParsingResult VnlcParser::parseVariableDeclarationStatement() {
    VnlcToken firstToken = peekValid();

    VnlcVariableDeclarationParsingContext context{
        .position = VnlcVariableDeclarationParsingContext::Position::STATEMENT,
        .hasMetadata = false,
        .metadataTerms = {},
    };

    auto result = parseVariableDeclaration(std::move(context));

    VnlcToken lastToken = peek();

    return VnlcVariableDeclarationStatementParsingResult{
        .statement = std::make_unique<VnlcVariableDeclarationStatementNode>(std::move(result.declaration), firstToken, lastToken),
    };
}

VnlcBlockStatementParsingResult VnlcParser::parseBlockStatement() {
    VnlcToken firstToken = peekValid();

    if (!match(VnlcTokenType::LEFT_BRACE)) {
        throw VnlcSyntaxError("Expected '{' to start block statement", peek().getLine(), peek().getColumn());
    }

    std::vector<std::unique_ptr<VnlcStatementNode>> statements;

    while (!match(VnlcTokenType::RIGHT_BRACE)) {
        auto statementResult = parseStatement();
        statements.push_back(std::move(statementResult.statement));
    }

    VnlcToken lastToken = peek();

    return VnlcBlockStatementParsingResult{
        .statement = std::make_unique<VnlcBlockStatementNode>(std::move(statements), firstToken, lastToken),
    };
}

VnlcControlFlowStatementParsingResult VnlcParser::parseControlFlowStatement() {
    if (check(VnlcTokenType::IF)) {
        auto result = parseIfStatement();

        return VnlcControlFlowStatementParsingResult{
            .statement = std::move(result.statement),
        };
    } else if (check(VnlcTokenType::SWITCH)) {
        auto result = parseSwitchStatement();

        return VnlcControlFlowStatementParsingResult{
            .statement = std::move(result.statement),
        };
    } else if (match(VnlcTokenType::LABEL)) {
        std::string label;

        if (!check(VnlcTokenType::IDENTIFIER)) {
            throw VnlcSyntaxError("Expected identifier after 'label'", peek().getLine(), peek().getColumn());
        } else {
            label = peek().getValue();
            advance();
        }

        if (check(VnlcTokenType::WHILE)) {
            VnlcWhileStatementParsingContext context{
                .label = std::move(label),
            };

            auto result = parseWhileStatement(std::move(context));

            return VnlcControlFlowStatementParsingResult{
                .statement = std::move(result.statement),
            };
        } else if (check(VnlcTokenType::FOR)) {
            VnlcForStatementParsingContext context{
                .label = std::move(label),
            };

            auto result = parseForStatement(std::move(context));

            return VnlcControlFlowStatementParsingResult{
                .statement = std::move(result.statement),
            };
        } else {
            throw VnlcSyntaxError("Expected 'while' or 'for' after label declaration", peek().getLine(), peek().getColumn());
        }
    } else if (check(VnlcTokenType::WHILE)) {
        auto result = parseWhileStatement({ std::nullopt });

        return VnlcControlFlowStatementParsingResult{
            .statement = std::move(result.statement),
        };
    } else if (check(VnlcTokenType::FOR)) {
        auto result = parseForStatement({ std::nullopt });

        return VnlcControlFlowStatementParsingResult{
            .statement = std::move(result.statement),
        };
    } else if (check(VnlcTokenType::RETURN)) {
        auto result = parseReturnStatement();

        return VnlcControlFlowStatementParsingResult{
            .statement = std::move(result.statement),
        };
    } else if (check(VnlcTokenType::BREAK)) {
        auto result = parseBreakStatement();

        return VnlcControlFlowStatementParsingResult{
            .statement = std::move(result.statement),
        };
    } else if (check(VnlcTokenType::CONTINUE)) {
        auto result = parseContinueStatement();

        return VnlcControlFlowStatementParsingResult{
            .statement = std::move(result.statement),
        };
    } else if (check(VnlcTokenType::RELOAD)) {
        auto result = parseReloadStatement();

        return VnlcControlFlowStatementParsingResult{
            .statement = std::move(result.statement),
        };
    } else {
        throw VnlcSyntaxError("Expected control flow statement", peek().getLine(), peek().getColumn());
    }
}

VnlcIfStatementParsingResult VnlcParser::parseIfStatement() {
    VnlcToken firstToken = peekValid();

    if (!match(VnlcTokenType::IF)) {
        throw VnlcSyntaxError("Expected 'if' to start if statement", peek().getLine(), peek().getColumn());
    }

    if (!match(VnlcTokenType::LEFT_PARENTHESIS)) {
        throw VnlcSyntaxError("Expected '(' after 'if'", peek().getLine(), peek().getColumn());
    }

    auto conditionResult = parseExpression();

    if (!match(VnlcTokenType::RIGHT_PARENTHESIS)) {
        throw VnlcSyntaxError("Expected ')' after if condition", peek().getLine(), peek().getColumn());
    }

    auto thenBranchResult = parseStatement();

    std::optional<std::unique_ptr<VnlcStatementNode>> elseBranch = std::nullopt;

    if (match(VnlcTokenType::ELSE)) {
        auto elseBranchResult = parseStatement();
        elseBranch = std::move(elseBranchResult.statement);
    }

    VnlcToken lastToken = peek();

    return VnlcIfStatementParsingResult{
        .statement = std::make_unique<VnlcIfStatementNode>(std::move(conditionResult.expression), std::move(thenBranchResult.statement), std::move(elseBranch), firstToken, lastToken),
    };
}

VnlcSwitchStatementParsingResult VnlcParser::parseSwitchStatement() {
    VnlcToken firstToken = peekValid();

    bool hasSwitchCases = false;
    VnlcSwitchStatementType switchType = VnlcSwitchStatementType::LITERAL_MATCH;
    std::vector<VnlcSwitchStatementItem::LiteralMatchItem> literalMatchItems;
    std::vector<VnlcSwitchStatementItem::TypeMatchItem> typeMatchItems;
    std::unique_ptr<VnlcStatementNode> defaultCase = nullptr;

    if (!match(VnlcTokenType::SWITCH)) {
        throw VnlcSyntaxError("Expected 'switch' to start switch statement", peek().getLine(), peek().getColumn());
    }

    if (!match(VnlcTokenType::LEFT_PARENTHESIS)) {
        throw VnlcSyntaxError("Expected '(' after 'switch'", peek().getLine(), peek().getColumn());
    }

    auto expressionResult = parseExpression();

    if (!match(VnlcTokenType::RIGHT_PARENTHESIS)) {
        throw VnlcSyntaxError("Expected ')' after switch expression", peek().getLine(), peek().getColumn());
    }

    if (!match(VnlcTokenType::LEFT_BRACE)) {
        throw VnlcSyntaxError("Expected '{' to start switch body", peek().getLine(), peek().getColumn());
    }

    while (check(VnlcTokenType::CASE)) {
        unsigned int caseBeginLine = peek().getLine();
        unsigned int caseBeginColumn = peek().getColumn();
        auto caseResult = parseSwitchCase();

        if (hasSwitchCases && caseResult.kind != switchType) {
            throw VnlcSyntaxError("Cannot mix different types of switch cases in the same switch statement", caseBeginLine, caseBeginColumn);
        }

        if (!hasSwitchCases) {
            switchType = caseResult.kind;
            hasSwitchCases = true;
        }

        if (!match(VnlcTokenType::ARROW)) {
            throw VnlcSyntaxError("Expected '->' after case label", peek().getLine(), peek().getColumn());
        }

        auto caseBodyResult = parseStatement();

        if (caseResult.kind == VnlcSwitchStatementType::LITERAL_MATCH) {
            literalMatchItems.push_back({ std::move(caseResult.literal.value()), std::move(caseBodyResult.statement) });
        } else if (caseResult.kind == VnlcSwitchStatementType::TYPE_MATCH) {
            typeMatchItems.push_back({ std::move(caseResult.type.value()), std::move(caseBodyResult.statement) });
        }
    }

    if (match(VnlcTokenType::DEFAULT)) {
        if (!match(VnlcTokenType::ARROW)) {
            throw VnlcSyntaxError("Expected '->' after 'default'", peek().getLine(), peek().getColumn());
        }

        auto defaultBodyResult = parseStatement();

        defaultCase = std::move(defaultBodyResult.statement);
    }

    if (!match(VnlcTokenType::RIGHT_BRACE)) {
        throw VnlcSyntaxError("Expected '}' to end switch body", peek().getLine(), peek().getColumn());
    }

    VnlcToken lastToken = peek();

    if (switchType == VnlcSwitchStatementType::LITERAL_MATCH) {
        return VnlcSwitchStatementParsingResult{
            .statement = std::make_unique<VnlcSwitchStatementNode>(std::move(expressionResult.expression), std::move(literalMatchItems), std::move(defaultCase), firstToken, lastToken),
        };
    } else {
        return VnlcSwitchStatementParsingResult{
            .statement = std::make_unique<VnlcSwitchStatementNode>(std::move(expressionResult.expression), std::move(typeMatchItems), std::move(defaultCase), firstToken, lastToken),
        };
    }
}

VnlcWhileStatementParsingResult VnlcParser::parseWhileStatement(VnlcWhileStatementParsingContext context) {
    VnlcToken firstToken = peekValid();

    if (!match(VnlcTokenType::WHILE)) {
        throw VnlcSyntaxError("Expected 'while' to start while statement", peek().getLine(), peek().getColumn());
    }

    if (!match(VnlcTokenType::LEFT_PARENTHESIS)) {
        throw VnlcSyntaxError("Expected '(' after 'while'", peek().getLine(), peek().getColumn());
    }

    auto conditionResult = parseExpression();

    if (!match(VnlcTokenType::RIGHT_PARENTHESIS)) {
        throw VnlcSyntaxError("Expected ')' after while condition", peek().getLine(), peek().getColumn());
    }

    auto bodyResult = parseStatement();

    VnlcToken lastToken = peek();

    if (context.label.has_value()) {
        return VnlcWhileStatementParsingResult{
            .statement =
                std::make_unique<VnlcWhileStatementNode>(std::move(context.label.value()), std::move(conditionResult.expression), std::move(bodyResult.statement), firstToken, lastToken),
        };
    } else {
        return VnlcWhileStatementParsingResult{
            .statement = std::make_unique<VnlcWhileStatementNode>(std::move(conditionResult.expression), std::move(bodyResult.statement), firstToken, lastToken),
        };
    }
}

VnlcForStatementParsingResult VnlcParser::parseForStatement(VnlcForStatementParsingContext context) {
    VnlcToken firstToken = peekValid();

    VnlcVariableDeclarationType loopVariableKind;
    std::string loopVariableName;
    std::optional<std::unique_ptr<VnlcTypeAnnotationNode>> loopVariableTypeAnnotation;
    std::unique_ptr<VnlcExpressionNode> iterableExpression;
    std::unique_ptr<VnlcStatementNode> body;

    if (!match(VnlcTokenType::FOR)) {
        throw VnlcSyntaxError("Expected 'for' to start for statement", peek().getLine(), peek().getColumn());
    }

    if (!match(VnlcTokenType::LEFT_PARENTHESIS)) {
        throw VnlcSyntaxError("Expected '(' after 'for'", peek().getLine(), peek().getColumn());
    }

    auto variableDeclarationPrimaryResult = parseVariableDeclarationPrimary();
    loopVariableKind = variableDeclarationPrimaryResult.type;
    loopVariableName = std::move(variableDeclarationPrimaryResult.name);
    loopVariableTypeAnnotation = std::move(variableDeclarationPrimaryResult.typeAnnotation);

    if (!match(VnlcTokenType::IN)) {
        throw VnlcSyntaxError("Expected 'in' after loop variable declaration", peek().getLine(), peek().getColumn());
    }

    auto iterableExpressionResult = parseExpression();
    iterableExpression = std::move(iterableExpressionResult.expression);

    if (!match(VnlcTokenType::RIGHT_PARENTHESIS)) {
        throw VnlcSyntaxError("Expected ')' after for statement", peek().getLine(), peek().getColumn());
    }

    auto bodyResult = parseStatement();
    body = std::move(bodyResult.statement);

    VnlcToken lastToken = peek();

    if (context.label.has_value()) {
        return VnlcForStatementParsingResult{
            .statement = std::make_unique<VnlcForStatementNode>(
                std::move(context.label.value()),
                loopVariableKind,
                std::move(loopVariableName),
                std::move(loopVariableTypeAnnotation),
                std::move(iterableExpression),
                std::move(body),
                firstToken,
                lastToken
            ),
        };
    } else {
        return VnlcForStatementParsingResult{
            .statement = std::make_unique<VnlcForStatementNode>(
                loopVariableKind,
                std::move(loopVariableName),
                std::move(loopVariableTypeAnnotation),
                std::move(iterableExpression),
                std::move(body),
                firstToken,
                lastToken
            ),
        };
    }
}

VnlcReturnStatementParsingResult VnlcParser::parseReturnStatement() {
    VnlcToken firstToken = peekValid();

    if (!match(VnlcTokenType::RETURN)) {
        throw VnlcSyntaxError("Expected 'return' to start return statement", peek().getLine(), peek().getColumn());
    }

    std::optional<std::unique_ptr<VnlcExpressionNode>> returnValue = std::nullopt;

    if (!(peek().getType() == VnlcTokenType::NEWLINE || peek().getType() == VnlcTokenType::RIGHT_BRACE)) {
        auto returnValueResult = parseExpression();
        returnValue = std::move(returnValueResult.expression);
    }

    VnlcToken lastToken = peek();

    if (returnValue.has_value()) {
        return VnlcReturnStatementParsingResult{
            .statement = std::make_unique<VnlcReturnStatementNode>(std::move(returnValue.value()), firstToken, lastToken),
        };
    } else {
        return VnlcReturnStatementParsingResult{
            .statement = std::make_unique<VnlcReturnStatementNode>(firstToken, lastToken),
        };
    }
}

VnlcBreakStatementParsingResult VnlcParser::parseBreakStatement() {
    VnlcToken firstToken = peekValid();

    if (!match(VnlcTokenType::BREAK)) {
        throw VnlcSyntaxError("Expected 'break' to start break statement", peek().getLine(), peek().getColumn());
    }

    std::optional<std::string> label = std::nullopt;

    if (!(peek().getType() == VnlcTokenType::NEWLINE || peek().getType() == VnlcTokenType::RIGHT_BRACE)) {
        if (!check(VnlcTokenType::IDENTIFIER)) {
            throw VnlcSyntaxError("Expected identifier after 'break'", peek().getLine(), peek().getColumn());
        } else {
            label = peek().getValue();
            advance();
        }
    }

    VnlcToken lastToken = peek();

    if (label.has_value()) {
        return VnlcBreakStatementParsingResult{
            .statement = std::make_unique<VnlcBreakStatementNode>(std::move(label.value()), firstToken, lastToken),
        };
    } else {
        return VnlcBreakStatementParsingResult{
            .statement = std::make_unique<VnlcBreakStatementNode>(firstToken, lastToken),
        };
    }
}

VnlcContinueStatementParsingResult VnlcParser::parseContinueStatement() {
    VnlcToken firstToken = peekValid();

    if (!match(VnlcTokenType::CONTINUE)) {
        throw VnlcSyntaxError("Expected 'continue' to start continue statement", peek().getLine(), peek().getColumn());
    }

    std::optional<std::string> label = std::nullopt;

    if (!(peek().getType() == VnlcTokenType::NEWLINE || peek().getType() == VnlcTokenType::RIGHT_BRACE)) {
        if (!check(VnlcTokenType::IDENTIFIER)) {
            throw VnlcSyntaxError("Expected identifier after 'continue'", peek().getLine(), peek().getColumn());
        } else {
            label = peek().getValue();
            advance();
        }
    }

    VnlcToken lastToken = peek();

    if (label.has_value()) {
        return VnlcContinueStatementParsingResult{
            .statement = std::make_unique<VnlcContinueStatementNode>(std::move(label.value()), firstToken, lastToken),
        };
    } else {
        return VnlcContinueStatementParsingResult{
            .statement = std::make_unique<VnlcContinueStatementNode>(firstToken, lastToken),
        };
    }
}

VnlcReloadStatementParsingResult VnlcParser::parseReloadStatement() {
    VnlcToken firstToken = peekValid();

    if (!match(VnlcTokenType::RELOAD)) {
        throw VnlcSyntaxError("Expected 'reload' to start reload statement", peek().getLine(), peek().getColumn());
    }

    VnlcToken lastToken = peek();

    return VnlcReloadStatementParsingResult{
        .statement = std::make_unique<VnlcReloadStatementNode>(firstToken, lastToken),
    };
}

VnlcSwitchCaseParsingResult VnlcParser::parseSwitchCase() {
    if (!match(VnlcTokenType::CASE)) {
        throw VnlcSyntaxError("Expected 'case' to start switch case", peek().getLine(), peek().getColumn());
    }

    static const std::unordered_set<VnlcTokenType> literalStarters = {
        VnlcTokenType::PLUS,
        VnlcTokenType::MINUS,
        VnlcTokenType::TILDE,
        VnlcTokenType::EXCLAMATION,
        VnlcTokenType::NUMBER,
        VnlcTokenType::STRING,
        VnlcTokenType::CHAR,
        VnlcTokenType::TRUE,
        VnlcTokenType::FALSE,
    };

    if (checkAny(literalStarters)) {
        auto literalExpressionResult = parseExpression();

        return VnlcSwitchCaseParsingResult{
            .kind = VnlcSwitchStatementType::LITERAL_MATCH,
            .literal = std::move(literalExpressionResult.expression),
        };
    } else if (check(VnlcTokenType::IDENTIFIER)) {
        auto typeResult = parseType();

        std::optional<std::unique_ptr<VnlcExpressionNode>> guardExpression = std::nullopt;
        if (match(VnlcTokenType::WHEN)) {
            auto guardResult = parseExpression();
            guardExpression = std::move(guardResult.expression);
        }

        return VnlcSwitchCaseParsingResult{
            .kind = VnlcSwitchStatementType::TYPE_MATCH,
            .type = std::move(typeResult.type),
            .guardExpression = std::move(guardExpression),
        };
    } else {
        throw VnlcSyntaxError("Expected literal or type identifier after 'case'", peek().getLine(), peek().getColumn());
    }
}
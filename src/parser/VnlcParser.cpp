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
#include <memory>
#include <optional>
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

bool VnlcParser::check(std::span<VnlcTokenType> expectedTypes) {
    for (unsigned int i = 0; i < expectedTypes.size(); i++) {
        skipNewlines();
        if (!hasNextToken() || peek(i).getType() != expectedTypes[i]) {
            return false;
        }
    }
    return true;
}

bool VnlcParser::checkGeneralizedIdentifier() {
    skipNewlines();
    if (hasNextToken() && VnlcTokenTypeUtil::isGeneralizedIdentifier(peek().getType())) {
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

bool VnlcParser::match(std::span<VnlcTokenType> expectedTypes) {
    if (check(expectedTypes)) {
        for (unsigned int i = 0; i < expectedTypes.size(); i++) {
            advance();
        }
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

VnlcFunctionDeclarationParsingResult VnlcParser::parseFunctionDeclaration(VnlcFunctionDeclarationParsingContext context) {
    VnlcToken firstToken = peek();

    if (check(VnlcTokenType::FUNC)) {
        VnlcRegularFunctionDeclarationParsingContext regularContext{
            .context = context.context,
            .hasMetadata = context.hasMetadata,
            .metadataTerms = std::move(context.metadataTerms),
        };
        auto result = parseRegularFunctionDeclaration(std::move(regularContext));

        VnlcToken lastToken = peek();
        result.declaration->resetPosition(firstToken, lastToken);

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
    VnlcToken firstToken = peek();

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
    VnlcToken firstToken = peek();

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
    VnlcToken firstToken = peek();

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
    VnlcToken firstToken = peek();

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

    VnlcToken firstToken = peek();

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

    VnlcToken firstToken = peek();

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

    VnlcToken firstToken = peek();

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

    VnlcToken firstToken = peek();

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

    std::array<VnlcTokenType, 2> expectedTypes = { VnlcTokenType::DOT, VnlcTokenType::IDENTIFIER };

    do {
        if (!check(VnlcTokenType::IDENTIFIER)) {
            throw VnlcSyntaxError("Expected identifier in import path", peek().getLine(), peek().getColumn());
        } else {
            namePartsPrefix.emplace_back(peek().getValue());
            advance();
        }
    } while (match(expectedTypes));

    if (match(VnlcTokenType::DOT)) {
        if (!match(VnlcTokenType::ASTERISK)) {
            throw VnlcSyntaxError("Expected '*' after '.' in wildcard import path", peek().getLine(), peek().getColumn());
        }

        std::vector<std::string> nameParts = std::move(namePartsPrefix);
        nameParts.emplace_back("*");

        paths.emplace_back(
            VnlcImportDeclarationItem{
                .nameParts = std::move(nameParts),
                .alias = std::nullopt,
            }
        );
    } else if (match(VnlcTokenType::AS)) {
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

    std::array<VnlcTokenType, 2> identifierMatch = { VnlcTokenType::DOT, VnlcTokenType::IDENTIFIER };
    std::array<VnlcTokenType, 2> parentMatch = { VnlcTokenType::DOT, VnlcTokenType::PARENT };

    do {
        if (match(VnlcTokenType::PARENT)) {
            namePartsPrefix.emplace_back("parent");
        } else if (!check(VnlcTokenType::IDENTIFIER)) {
            throw VnlcSyntaxError("Expected identifier or 'parent' in relative import path", peek().getLine(), peek().getColumn());
        } else {
            namePartsPrefix.emplace_back(peek().getValue());
            advance();
        }
    } while (match(parentMatch) || match(identifierMatch));

    if (match(VnlcTokenType::DOT)) {
        if (!match(VnlcTokenType::ASTERISK)) {
            throw VnlcSyntaxError("Expected '*' after '.' in wildcard import path", peek().getLine(), peek().getColumn());
        }

        std::vector<std::string> nameParts = std::move(namePartsPrefix);
        nameParts.emplace_back("*");

        paths.emplace_back(
            VnlcImportDeclarationItem{
                .nameParts = std::move(nameParts),
                .alias = std::nullopt,
            }
        );
    } else if (match(VnlcTokenType::AS)) {
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
    VnlcToken firstToken = peek();

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
    VnlcToken firstToken = peek();

    auto result = parseBlockStatement();

    VnlcToken lastToken = peek();
    result.statement->resetPosition(firstToken, lastToken);

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
    VnlcToken firstToken = peek();

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

        return VnlcClassMemberParsingResult{
            .declaration = std::move(propertyDeclarationResult.declaration),
        };
    }
}

VnlcEnumMemberDeclarationParsingResult VnlcParser::parseEnumMemberDeclaration() {
    VnlcToken firstToken = peek();

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
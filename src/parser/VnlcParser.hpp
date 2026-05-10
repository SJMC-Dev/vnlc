#pragma once

#ifndef VNLC_PARSER_HPP
#define VNLC_PARSER_HPP

#include "../ast/declaration/VnlcDeclarationItem.hpp"
#include "../ast/declaration/VnlcDeclarationNode.hpp"
#include "../ast/declaration/VnlcExportDeclarationNode.hpp"
#include "../ast/declaration/VnlcFunctionDeclarationNode.hpp"
#include "../ast/declaration/VnlcImportDeclarationNode.hpp"
#include "../ast/declaration/VnlcPropertyDeclarationNode.hpp"
#include "../ast/declaration/VnlcTypeDeclarationNode.hpp"
#include "../ast/declaration/VnlcVariableDeclarationNode.hpp"
#include "../ast/declaration/VnlcClassDeclarationNode.hpp"
#include "../ast/declaration/VnlcInterfaceDeclarationNode.hpp"
#include "../ast/declaration/VnlcEnumDeclarationNode.hpp"
#include "../ast/declaration/VnlcTypeAliasDeclarationNode.hpp"
#include "../ast/module/VnlcModuleNode.hpp"
#include "../ast/other/VnlcTypeAnnotationNode.hpp"
#include "../config/VnlcConfig.hpp"
#include "../lexer/VnlcLexer.hpp"
#include "../token/VnlcToken.hpp"
#include <memory>
#include <vector>

class VnlcParser {
private:
    VnlcLexer lexer;

    std::vector<VnlcToken> tokenBuffer;
    unsigned int currentTokenIndex;

    [[nodiscard]] bool hasNextToken() const;
    [[nodiscard]] const VnlcToken& peek() const;
    [[nodiscard]] const VnlcToken& peek(unsigned int offset) const;
    void advance();

    [[nodiscard]] bool match(VnlcTokenType expectedType) const;

    // TODO: specify parameters and return types for each parsing function, use void before we determine them
    [[nodiscard]] std::unique_ptr<VnlcModuleNode> parseModule(const VnlcConfig& config);
    [[nodiscard]] std::unique_ptr<VnlcDeclarationNode> parseTopIdentifierDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcImportDeclarationNode> parseImportDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcExportDeclarationNode> parseExportDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcVariableDeclarationNode> parseVariableDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcFunctionDeclarationNode> parseFunctionDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcTypeDeclarationNode> parseTypeDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcPropertyDeclarationNode> parsePropertyDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcFunctionDeclarationNode> parseClassMethodDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcFunctionDeclarationNode> parseInterfaceMethodDeclaration();
    [[nodiscard]] std::vector<VnlcDeclarationItem::MetadataTerm> parseMetadata();
    [[nodiscard]] std::unique_ptr<VnlcFunctionDeclarationNode> parseRegularFunctionDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcFunctionDeclarationNode> parseNativeFunctionDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcTypeAnnotationNode> parseTypeAnnotation();
    [[nodiscard]] std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>> parseParameterList();
    [[nodiscard]] std::unique_ptr<VnlcClassDeclarationNode> parseClassDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcInterfaceDeclarationNode> parseInterfaceDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcEnumDeclarationNode> parseEnumDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcTypeAliasDeclarationNode> parseTypeAliasDeclaration();
    void parseImportPath();
    void parseExportList();
    void parseMetadataTerm();
    void parseAbsoluteImportPath();
    void parseRelativeImportPath();
    void parseType();
    void parseParameter();
    void parseGenericParameterList();
    void parseGenericArgumentList();
    void parseAbsoluteImportPathList();
    void parseRelativeImportPathList();
    void parseFunctionBody();
    void parseClassBody();
    void parseInterfaceBody();
    void parseEnumBody();
    void parseAbsoluteImportPathItem();
    void parseRelativeImportPathItem();
    void parseClassMember();
    void parseInterfaceMember();
    void parseEnumMemberDeclaration();
    void parseEnumAssociatedValueList();
    void parseEnumAssociatedValue();
    void parseExpression();
    void parseAssignmentExpression();
    void parseConditionalExpression();
    void parseNullishCoalescingExpression();
    void parseLogicalOrExpression();
    void parseLogicalAndExpression();
    void parseEqualityExpression();
    void parseRelationalExpression();
    void parseBitwiseOrExpression();
    void parseBitwiseXorExpression();
    void parseBitwiseAndExpression();
    void parseShiftExpression();
    void parseRangeExpression();
    void parseAdditiveExpression();
    void parseMultiplicativeExpression();
    void parseUnaryExpression();
    void parseExponentialExpression();
    void parsePostfixExpression();
    void parsePrimaryExpression();
    void parseAssignmentOperator();
    void parsePostfixSuffix();
    void parseLiteral();
    void parseString();
    void parseBoolean();
    void parseListLiteral();
    void parseDictLiteral();
    void parseSNBTArray();
    void parseSelector();
    void parseArgumentList();
    void parseInterpolation();
    void parseStringPart();
    void parseDictEntry();
    void parseSelectorArgumentList();
    void parseSelectorArgument();
    void parseStatement();
    void parseExpressionStatement();
    void parseVariableDeclarationStatement();
    void parseBlockStatement();
    void parseControlFlowStatement();
    void parseIfStatement();
    void parseSwitchStatement();
    void parseWhileStatement();
    void parseForStatement();
    void parseReturnStatement();
    void parseBreakStatement();
    void parseContinueStatement();
    void parseReloadStatement();
    void parseSwitchCase();

public:
    explicit VnlcParser(VnlcLexer&& lexer, unsigned int bufferSize = 2);

    [[nodiscard]] std::unique_ptr<VnlcModuleNode> parse(const VnlcConfig& config);
};

#endif // VNLC_PARSER_HPP

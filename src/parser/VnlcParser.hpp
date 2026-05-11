#pragma once

#ifndef VNLC_PARSER_HPP
#define VNLC_PARSER_HPP

#include "../ast/declaration/VnlcClassDeclarationNode.hpp"
#include "../ast/declaration/VnlcDeclarationItem.hpp"
#include "../ast/declaration/VnlcDeclarationNode.hpp"
#include "../ast/declaration/VnlcEnumDeclarationNode.hpp"
#include "../ast/declaration/VnlcEnumMemberDeclarationNode.hpp"
#include "../ast/declaration/VnlcExportDeclarationItem.hpp"
#include "../ast/declaration/VnlcExportDeclarationNode.hpp"
#include "../ast/declaration/VnlcFunctionDeclarationNode.hpp"
#include "../ast/declaration/VnlcImportDeclarationItem.hpp"
#include "../ast/declaration/VnlcImportDeclarationNode.hpp"
#include "../ast/declaration/VnlcInterfaceDeclarationNode.hpp"
#include "../ast/declaration/VnlcPropertyDeclarationNode.hpp"
#include "../ast/declaration/VnlcTypeAliasDeclarationNode.hpp"
#include "../ast/declaration/VnlcTypeDeclarationNode.hpp"
#include "../ast/declaration/VnlcVariableDeclarationNode.hpp"
#include "../ast/expression/VnlcBinaryExpressionNode.hpp"
#include "../ast/expression/VnlcConditionalExpressionNode.hpp"
#include "../ast/expression/VnlcDictLiteralExpressionNode.hpp"
#include "../ast/expression/VnlcExpressionNode.hpp"
#include "../ast/expression/VnlcListLikeLiteralExpressionNode.hpp"
#include "../ast/expression/VnlcLiteralExpressionNode.hpp"
#include "../ast/expression/VnlcRangeExpressionNode.hpp"
#include "../ast/expression/VnlcSelectorLiteralExpressionNode.hpp"
#include "../ast/expression/VnlcSimpleLiteralExpressionNode.hpp"
#include "../ast/expression/VnlcStringLiteralExpressionNode.hpp"
#include "../ast/expression/VnlcUnaryExpressionNode.hpp"
#include "../ast/module/VnlcModuleNode.hpp"
#include "../ast/other/VnlcTypeAnnotationNode.hpp"
#include "../ast/other/VnlcTypeNode.hpp"
#include "../ast/statement/VnlcBlockStatementNode.hpp"
#include "../ast/statement/VnlcBreakStatementNode.hpp"
#include "../ast/statement/VnlcContinueStatementNode.hpp"
#include "../ast/statement/VnlcControlFlowStatementNode.hpp"
#include "../ast/statement/VnlcExpressionStatementNode.hpp"
#include "../ast/statement/VnlcForStatementNode.hpp"
#include "../ast/statement/VnlcIfStatementNode.hpp"
#include "../ast/statement/VnlcReloadStatementNode.hpp"
#include "../ast/statement/VnlcReturnStatementNode.hpp"
#include "../ast/statement/VnlcStatementNode.hpp"
#include "../ast/statement/VnlcSwitchStatementItem.hpp"
#include "../ast/statement/VnlcSwitchStatementNode.hpp"
#include "../ast/statement/VnlcVariableDeclarationStatementNode.hpp"
#include "../ast/statement/VnlcWhileStatementNode.hpp"
#include "../config/VnlcConfig.hpp"
#include "../lexer/VnlcLexer.hpp"
#include "../token/VnlcToken.hpp"
#include <memory>
#include <span>
#include <utility>
#include <variant>
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
    [[nodiscard]] bool match(std::span<VnlcTokenType> expectedTypes) const;

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
    [[nodiscard]] std::vector<VnlcImportDeclarationItem> parseImportPath();
    [[nodiscard]] std::vector<VnlcExportDeclarationItem> parseExportList();
    [[nodiscard]] VnlcDeclarationItem::MetadataTerm parseMetadataTerm();
    [[nodiscard]] std::vector<VnlcImportDeclarationItem> parseAbsoluteImportPath();
    [[nodiscard]] std::vector<VnlcImportDeclarationItem> parseRelativeImportPath();
    [[nodiscard]] std::unique_ptr<VnlcTypeNode> parseType();
    [[nodiscard]] std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>> parseParameter();
    [[nodiscard]] std::vector<std::string> parseGenericParameterList();
    [[nodiscard]] std::vector<std::unique_ptr<VnlcTypeNode>> parseGenericArgumentList();
    [[nodiscard]] std::vector<VnlcImportDeclarationItem> parseAbsoluteImportPathList();
    [[nodiscard]] std::vector<VnlcImportDeclarationItem> parseRelativeImportPathList();
    [[nodiscard]] std::unique_ptr<VnlcBlockStatementNode> parseFunctionBody();
    [[nodiscard]] std::vector<std::unique_ptr<VnlcDeclarationNode>> parseClassBody();
    [[nodiscard]] std::vector<std::unique_ptr<VnlcFunctionDeclarationNode>> parseInterfaceBody();
    [[nodiscard]] std::vector<std::unique_ptr<VnlcEnumMemberDeclarationNode>> parseEnumBody();
    [[nodiscard]] std::vector<VnlcImportDeclarationItem> parseAbsoluteImportPathItem();
    [[nodiscard]] std::vector<VnlcImportDeclarationItem> parseRelativeImportPathItem();
    [[nodiscard]] std::unique_ptr<VnlcDeclarationNode> parseClassMember();
    [[nodiscard]] std::unique_ptr<VnlcEnumMemberDeclarationNode> parseEnumMemberDeclaration();
    [[nodiscard]] std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>> parseEnumAssociatedValueList();
    [[nodiscard]] std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>> parseEnumAssociatedValue();
    [[nodiscard]] std::unique_ptr<VnlcExpressionNode> parseExpression();
    [[nodiscard]] std::unique_ptr<VnlcBinaryExpressionNode> parseAssignmentExpression();
    [[nodiscard]] std::unique_ptr<VnlcConditionalExpressionNode> parseConditionalExpression();
    [[nodiscard]] std::unique_ptr<VnlcBinaryExpressionNode> parseNullishCoalescingExpression();
    [[nodiscard]] std::unique_ptr<VnlcBinaryExpressionNode> parseLogicalOrExpression();
    [[nodiscard]] std::unique_ptr<VnlcBinaryExpressionNode> parseLogicalAndExpression();
    [[nodiscard]] std::unique_ptr<VnlcBinaryExpressionNode> parseEqualityExpression();
    [[nodiscard]] std::unique_ptr<VnlcBinaryExpressionNode> parseRelationalExpression();
    [[nodiscard]] std::unique_ptr<VnlcBinaryExpressionNode> parseBitwiseOrExpression();
    [[nodiscard]] std::unique_ptr<VnlcBinaryExpressionNode> parseBitwiseXorExpression();
    [[nodiscard]] std::unique_ptr<VnlcBinaryExpressionNode> parseBitwiseAndExpression();
    [[nodiscard]] std::unique_ptr<VnlcBinaryExpressionNode> parseShiftExpression();
    [[nodiscard]] std::unique_ptr<VnlcRangeExpressionNode> parseRangeExpression();
    [[nodiscard]] std::unique_ptr<VnlcBinaryExpressionNode> parseAdditiveExpression();
    [[nodiscard]] std::unique_ptr<VnlcBinaryExpressionNode> parseMultiplicativeExpression();
    [[nodiscard]] std::unique_ptr<VnlcUnaryExpressionNode> parseUnaryExpression();
    [[nodiscard]] std::unique_ptr<VnlcBinaryExpressionNode> parseExponentialExpression();
    [[nodiscard]] std::unique_ptr<VnlcExpressionNode> parsePostfixExpression();
    [[nodiscard]] std::unique_ptr<VnlcExpressionNode> parsePrimaryExpression();
    [[nodiscard]] VnlcToken parseAssignmentOperator();
    [[nodiscard]] std::unique_ptr<VnlcExpressionNode> parsePostfixSuffix();
    [[nodiscard]] std::unique_ptr<VnlcLiteralExpressionNode> parseLiteral();
    [[nodiscard]] std::unique_ptr<VnlcStringLiteralExpressionNode> parseString();
    [[nodiscard]] std::unique_ptr<VnlcSimpleLiteralExpressionNode> parseBoolean();
    [[nodiscard]] std::unique_ptr<VnlcListLikeLiteralExpressionNode> parseListLiteral();
    [[nodiscard]] std::unique_ptr<VnlcDictLiteralExpressionNode> parseDictLiteral();
    [[nodiscard]] std::unique_ptr<VnlcListLikeLiteralExpressionNode> parseSNBTArray();
    [[nodiscard]] std::unique_ptr<VnlcSelectorLiteralExpressionNode> parseSelector();
    [[nodiscard]] std::vector<std::unique_ptr<VnlcExpressionNode>> parseArgumentList();
    [[nodiscard]] std::unique_ptr<VnlcExpressionNode> parseInterpolation();
    [[nodiscard]] std::variant<std::string, std::unique_ptr<VnlcExpressionNode>> parseStringPart();
    [[nodiscard]] std::pair<std::string, std::unique_ptr<VnlcExpressionNode>> parseDictEntry();
    [[nodiscard]] std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>> parseSelectorArgumentList();
    [[nodiscard]] std::pair<std::string, std::unique_ptr<VnlcExpressionNode>> parseSelectorArgument();
    [[nodiscard]] std::unique_ptr<VnlcStatementNode> parseStatement();
    [[nodiscard]] std::unique_ptr<VnlcExpressionStatementNode> parseExpressionStatement();
    [[nodiscard]] std::unique_ptr<VnlcVariableDeclarationStatementNode> parseVariableDeclarationStatement();
    [[nodiscard]] std::unique_ptr<VnlcBlockStatementNode> parseBlockStatement();
    [[nodiscard]] std::unique_ptr<VnlcControlFlowStatementNode> parseControlFlowStatement();
    [[nodiscard]] std::unique_ptr<VnlcIfStatementNode> parseIfStatement();
    [[nodiscard]] std::unique_ptr<VnlcSwitchStatementNode> parseSwitchStatement();
    [[nodiscard]] std::unique_ptr<VnlcWhileStatementNode> parseWhileStatement();
    [[nodiscard]] std::unique_ptr<VnlcForStatementNode> parseForStatement();
    [[nodiscard]] std::unique_ptr<VnlcReturnStatementNode> parseReturnStatement();
    [[nodiscard]] std::unique_ptr<VnlcBreakStatementNode> parseBreakStatement();
    [[nodiscard]] std::unique_ptr<VnlcContinueStatementNode> parseContinueStatement();
    [[nodiscard]] std::unique_ptr<VnlcReloadStatementNode> parseReloadStatement();
    [[nodiscard]] std::variant<VnlcSwitchStatementItem::LiteralMatchItem, VnlcSwitchStatementItem::TypeMatchItem> parseSwitchCase();

public:
    explicit VnlcParser(VnlcLexer&& lexer, unsigned int bufferSize = 2);

    [[nodiscard]] std::unique_ptr<VnlcModuleNode> parse(const VnlcConfig& config);
};

#endif // VNLC_PARSER_HPP

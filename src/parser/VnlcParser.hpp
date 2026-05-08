#pragma once

#ifndef VNLC_PARSER_HPP
#define VNLC_PARSER_HPP

#include "../ast/VnlcAstNode.hpp"
#include "../ast/declaration/VnlcClassDeclarationNode.hpp"
#include "../ast/declaration/VnlcDeclarationNode.hpp"
#include "../ast/declaration/VnlcEnumDeclarationNode.hpp"
#include "../ast/declaration/VnlcEnumMemberDeclarationNode.hpp"
#include "../ast/declaration/VnlcExportDeclarationNode.hpp"
#include "../ast/declaration/VnlcFunctionDeclarationNode.hpp"
#include "../ast/declaration/VnlcImportDeclarationNode.hpp"
#include "../ast/declaration/VnlcInterfaceDeclarationNode.hpp"
#include "../ast/declaration/VnlcPropertyDeclarationNode.hpp"
#include "../ast/declaration/VnlcTypeAliasDeclarationNode.hpp"
#include "../ast/declaration/VnlcTypeDeclarationNode.hpp"
#include "../ast/declaration/VnlcVariableDeclarationNode.hpp"
#include "../ast/module/VnlcModuleNode.hpp"
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

    // Parsing methods for module
    [[nodiscard]] std::unique_ptr<VnlcModuleNode> parseModule();

    // Parsing methods for declarations
    [[nodiscard]] std::unique_ptr<VnlcDeclarationNode> parseTopIdentifierDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcDeclarationNode> parseMemberDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcImportDeclarationNode> parseImportDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcExportDeclarationNode> parseExportDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcFunctionDeclarationNode> parseFunctionDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcVariableDeclarationNode> parseVariableDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcTypeDeclarationNode> parseTypeDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcClassDeclarationNode> parseClassDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcInterfaceDeclarationNode> parseInterfaceDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcEnumDeclarationNode> parseEnumDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcTypeAliasDeclarationNode> parseTypeAliasDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcPropertyDeclarationNode> parsePropertyDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcFunctionDeclarationNode> parseClassMethodDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcFunctionDeclarationNode> parseInterfaceMethodDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcEnumMemberDeclarationNode> parseEnumMemberDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcFunctionDeclarationNode> parseRegularFunctionDeclaration();
    [[nodiscard]] std::unique_ptr<VnlcFunctionDeclarationNode> parseNativeFunctionDeclaration();

    // Parsing methods for expressions
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseAssignmentExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseConditionalExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseNullishCoalescingExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseLogicalOrExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseLogicalAndExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseEqualityExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseRelationalExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseBitwiseOrExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseBitwiseXorExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseBitwiseAndExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseShiftExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseRangeExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseAdditiveExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseMultiplicativeExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseUnaryExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseExponentialExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parsePostfixExpression();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parsePrimaryExpression();

    // Parsing methods for statements
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseStatement();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseExpressionStatement();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseVariableDeclarationStatement();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseBlockStatement();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseControlFlowStatement();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseIfStatement();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseSwitchStatement();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseWhileStatement();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseForStatement();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseReturnStatement();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseBreakStatement();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseContinueStatement();
    [[nodiscard]] std::unique_ptr<VnlcAstNode> parseReloadStatement();

    // Parsing methods for

public:
    explicit VnlcParser(VnlcLexer&& lexer, unsigned int bufferSize = 2);

    [[nodiscard]] std::unique_ptr<VnlcAstNode> parse();
};

#endif // VNLC_PARSER_HPP

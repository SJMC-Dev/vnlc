#pragma once

#ifndef VNLC_PARSER_HPP
#define VNLC_PARSER_HPP

#include "../ast/module/VnlcModuleNode.hpp"
#include "../lexer/VnlcLexer.hpp"
#include "../token/VnlcToken.hpp"
#include <memory>
#include <vector>
#include "../config/VnlcConfig.hpp"

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
    void parseTopIdentifierDeclaration();
    void parseMemberDeclaration();
    void parseImportDeclaration();
    void parseExportDeclaration();
    void parseVariableDeclaration();
    void parseFunctionDeclaration();
    void parseTypeDeclaration();
    void parsePropertyDeclaration();
    void parseClassMethodDeclaration();
    void parseInterfaceMethodDeclaration();
    void parseMetadata();
    void parseRegularFunctionDeclaration();
    void parseNativeFunctionDeclaration();
    void parseTypeAnnotation();
    void parseParameterList();
    void parseClassDeclaration();
    void parseInterfaceDeclaration();
    void parseEnumDeclaration();
    void parseTypeAliasDeclaration();
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

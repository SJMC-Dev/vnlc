#pragma once

#ifndef VNLC_SEMANTIC_ANALYZER_HPP
#define VNLC_SEMANTIC_ANALYZER_HPP

#include "../ast/declaration/VnlcDeclarationNode.hpp"
#include "../ast/expression/VnlcExpressionNode.hpp"
#include "../ast/module/VnlcModuleNode.hpp"
#include "../ast/other/VnlcTypeNode.hpp"
#include "../ast/statement/VnlcStatementNode.hpp"
#include "VnlcSemanticAnalysisResult.hpp"
#include "VnlcSemanticContext.hpp"
#include "type/VnlcSemanticType.hpp"

class VnlcSemanticAnalyzer {
private:
    const VnlcModuleNode* module;
    VnlcSemanticContext context;

    void resolveImport(const VnlcImportDeclarationNode& importDecl);
    void checkDeclaration(const VnlcDeclarationNode& declaration);
    void checkStatement(const VnlcStatementNode& statement);
    void checkExpression(const VnlcExpressionNode& expression);
    void checkType(const VnlcTypeNode& type);

    [[nodiscard]] VnlcSemanticType inferExpressionType(const VnlcExpressionNode& expression);
    [[nodiscard]] VnlcSemanticType inferFunctionReturnType(const VnlcFunctionDeclarationNode& funcDecl);

public:
    explicit VnlcSemanticAnalyzer(const VnlcModuleNode* module);

    VnlcSemanticAnalyzer() = delete;
    VnlcSemanticAnalyzer(const VnlcSemanticAnalyzer&) = delete;
    VnlcSemanticAnalyzer& operator=(const VnlcSemanticAnalyzer&) = delete;

    VnlcSemanticAnalyzer(VnlcSemanticAnalyzer&&) noexcept = default;
    VnlcSemanticAnalyzer& operator=(VnlcSemanticAnalyzer&&) noexcept = default;

    [[nodiscard]] VnlcSemanticAnalysisResult analyze();
};

#endif // VNLC_SEMANTIC_ANALYZER_HPP
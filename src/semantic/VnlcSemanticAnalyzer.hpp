#pragma once

#ifndef VNLC_SEMANTIC_ANALYZER_HPP
#define VNLC_SEMANTIC_ANALYZER_HPP

#include "VnlcSemanticAnalysisResult.hpp"
#include "VnlcSemanticContext.hpp"
#include "../ast/module/VnlcModuleNode.hpp"
#include "../ast/declaration/VnlcDeclarationNode.hpp"
#include "../ast/statement/VnlcStatementNode.hpp"
#include "../ast/expression/VnlcExpressionNode.hpp"
#include "../ast/other/VnlcTypeNode.hpp"

class VnlcSemanticAnalyzer {
private:
    const VnlcModuleNode* module;
    VnlcSemanticContext context;

public:
    explicit VnlcSemanticAnalyzer(const VnlcModuleNode* module);

    VnlcSemanticAnalyzer() = delete;
    VnlcSemanticAnalyzer(const VnlcSemanticAnalyzer&) = delete;
    VnlcSemanticAnalyzer& operator=(const VnlcSemanticAnalyzer&) = delete;

    VnlcSemanticAnalyzer(VnlcSemanticAnalyzer&&) noexcept = default;
    VnlcSemanticAnalyzer& operator=(VnlcSemanticAnalyzer&&) noexcept = default;

    [[nodiscard]] VnlcSemanticAnalysisResult analyze();

private:
    void collectTopLevelSymbols();
    void resolveImports();
    void collectTypeMembers();
    void checkDeclarations();
    void checkExports();

    void checkDeclaration(const VnlcDeclarationNode& declaration);
    void checkStatement(const VnlcStatementNode& statement);
    void checkExpression(const VnlcExpressionNode& expression);
    void checkType(const VnlcTypeNode& type);
};

#endif // VNLC_SEMANTIC_ANALYZER_HPP
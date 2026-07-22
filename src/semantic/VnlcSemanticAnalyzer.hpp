#ifndef VNLC_SEMANTIC_ANALYZER_HPP
#define VNLC_SEMANTIC_ANALYZER_HPP

#include "../ast/declaration/VnlcClassDeclarationNode.hpp"
#include "../ast/declaration/VnlcEnumDeclarationNode.hpp"
#include "../ast/declaration/VnlcExportDeclarationNode.hpp"
#include "../ast/declaration/VnlcFunctionDeclarationNode.hpp"
#include "../ast/declaration/VnlcImportDeclarationNode.hpp"
#include "../ast/declaration/VnlcInterfaceDeclarationNode.hpp"
#include "../ast/declaration/VnlcTypeAliasDeclarationNode.hpp"
#include "../ast/declaration/VnlcValueDeclarationNode.hpp"
#include "../ast/expression/VnlcExpressionNode.hpp"
#include "../ast/expression/VnlcIdentifierExpressionNode.hpp"
#include "../ast/module/VnlcModuleNode.hpp"
#include "../ast/statement/VnlcStatementNode.hpp"
#include "../ast/type/VnlcTypeNode.hpp"
#include "../config/VnlcConfig.hpp"
#include "VnlcSemanticAnalysisResult.hpp"
#include "VnlcSemanticContext.hpp"
#include "type/typeinf/VnlcTypeInferenceResult.hpp"

class VnlcSemanticAnalyzer {
private:
    const VnlcModuleNode& module;
    VnlcSemanticContext context;

    void checkIdentifierName(std::string_view name, const VnlcDeclarationNode& declNode);
    void checkIdentifierExpressionUse(const VnlcIdentifierExpressionNode& exprNode);

    void checkModule(const VnlcModuleNode& moduleNode);
    void checkImport(const VnlcImportDeclarationNode& importDecl);
    void checkExport(const VnlcExportDeclarationNode& exportDecl);
    void checkVariableDeclaration(const VnlcValueDeclarationNode& varDecl);
    void checkFunctionDeclaration(const VnlcFunctionDeclarationNode& funcDecl);
    void checkClassDeclaration(const VnlcClassDeclarationNode& classDecl);
    void checkInterfaceDeclaration(const VnlcInterfaceDeclarationNode& interfaceDecl);
    void checkEnumDeclaration(const VnlcEnumDeclarationNode& enumDecl);
    void checkTypeAliasDeclaration(const VnlcTypeAliasDeclarationNode& typeAliasDecl);

    void checkStatement(const VnlcStatementNode& statement);
    void checkExpression(const VnlcExpressionNode& expression);
    void checkType(const VnlcTypeNode& type);

    [[nodiscard]] VnlcTypeInferenceResult inferExpressionType(const VnlcExpressionNode& expression);
    [[nodiscard]] VnlcTypeInferenceResult inferFunctionReturnType(const VnlcFunctionDeclarationNode& funcDecl);

public:
    explicit VnlcSemanticAnalyzer(const VnlcModuleNode& module);

    VnlcSemanticAnalyzer() = delete;
    VnlcSemanticAnalyzer(const VnlcSemanticAnalyzer&) = delete;
    VnlcSemanticAnalyzer& operator=(const VnlcSemanticAnalyzer&) = delete;

    VnlcSemanticAnalyzer(VnlcSemanticAnalyzer&&) noexcept = default;
    VnlcSemanticAnalyzer& operator=(VnlcSemanticAnalyzer&&) noexcept = delete;

    [[nodiscard]] VnlcSemanticAnalysisResult analyze(const VnlcConfig& config);
};

#endif // VNLC_SEMANTIC_ANALYZER_HPP
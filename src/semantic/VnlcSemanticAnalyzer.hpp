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

    bool checkIdentifierName(std::string_view name, const VnlcDeclarationNode& declNode);
    bool checkIdentifierExpressionUse(const VnlcIdentifierExpressionNode& exprNode);

    bool checkModule(const VnlcModuleNode& moduleNode);
    bool checkImport(const VnlcImportDeclarationNode& importDecl);
    bool checkExport(const VnlcExportDeclarationNode& exportDecl);
    bool checkVariableDeclaration(const VnlcValueDeclarationNode& varDecl);
    bool checkFunctionDeclaration(const VnlcFunctionDeclarationNode& funcDecl);
    bool checkClassDeclaration(const VnlcClassDeclarationNode& classDecl);
    bool checkInterfaceDeclaration(const VnlcInterfaceDeclarationNode& interfaceDecl);
    bool checkEnumDeclaration(const VnlcEnumDeclarationNode& enumDecl);
    bool checkTypeAliasDeclaration(const VnlcTypeAliasDeclarationNode& typeAliasDecl);

    bool checkStatement(const VnlcStatementNode& statement);
    bool checkExpression(const VnlcExpressionNode& expression);
    bool checkType(const VnlcTypeNode& type);

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
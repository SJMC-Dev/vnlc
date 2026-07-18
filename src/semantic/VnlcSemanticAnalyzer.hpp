#ifndef VNLC_SEMANTIC_ANALYZER_HPP
#define VNLC_SEMANTIC_ANALYZER_HPP

#include "../ast/declaration/VnlcExportDeclarationNode.hpp"
#include "../ast/declaration/VnlcImportDeclarationNode.hpp"
#include "../ast/declaration/VnlcTypeAliasDeclarationNode.hpp"
#include "../ast/declaration/VnlcVariableDeclarationNode.hpp"
#include "../ast/expression/VnlcExpressionNode.hpp"
#include "../ast/module/VnlcModuleNode.hpp"
#include "../ast/statement/VnlcStatementNode.hpp"
#include "../ast/type/VnlcTypeNode.hpp"
#include "../config/VnlcConfig.hpp"
#include "VnlcSemanticAnalysisResult.hpp"
#include "VnlcSemanticContext.hpp"
#include "type/VnlcSemanticType.hpp"

class VnlcSemanticAnalyzer {
private:
    const VnlcModuleNode& module;
    VnlcSemanticContext context;

    void checkModule(const VnlcModuleNode& moduleNode);
    void checkImport(const VnlcImportDeclarationNode& importDecl);
    void checkExport(const VnlcExportDeclarationNode& exportDecl);
    void checkVariableDeclaration(const VnlcVariableDeclarationNode& varDecl);
    void checkFunctionDeclaration(const VnlcFunctionDeclarationNode& funcDecl);
    void checkClassDeclaration(const VnlcClassDeclarationNode& classDecl);
    void checkInterfaceDeclaration(const VnlcInterfaceDeclarationNode& interfaceDecl);
    void checkEnumDeclaration(const VnlcEnumDeclarationNode& enumDecl);
    void checkTypeAliasDeclaration(const VnlcTypeAliasDeclarationNode& typeAliasDecl);
    void checkStatement(const VnlcStatementNode& statement);
    void checkExpression(const VnlcExpressionNode& expression);
    void checkType(const VnlcTypeNode& type);

    [[nodiscard]] VnlcSemanticType inferExpressionType(const VnlcExpressionNode& expression);
    [[nodiscard]] VnlcSemanticType inferFunctionReturnType(const VnlcFunctionDeclarationNode& funcDecl);

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
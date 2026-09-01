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
#include "metadata/VnlcMetadataInfo.hpp"
#include "../type/typeinf/VnlcTypeInferenceResult.hpp"

class VnlcSemanticAnalyzer {
private:
    const VnlcModuleNode& module;
    VnlcSemanticContext context;

    void checkIdentifierExpressionUse(const VnlcIdentifierExpressionNode& exprNode, VnlcMetadataInfo metadataInfo = VnlcMetadataInfo::DEFAULT);

    [[nodiscard]] VnlcMetadataInfo checkMetadata(const std::vector<VnlcDeclarationItem::MetadataTerm>& metadataTerms, const VnlcDeclarationNode& declNode);

    void checkModule(const VnlcModuleNode& moduleNode, const VnlcConfig& config);
    void checkImport(const VnlcImportDeclarationNode& importDecl, const VnlcConfig& config);
    void checkExport(const VnlcExportDeclarationNode& exportDecl);
    void checkValueDeclaration(const VnlcValueDeclarationNode& varDecl, VnlcMetadataInfo metadataInfo = VnlcMetadataInfo::DEFAULT);
    void checkFunctionDeclaration(const VnlcFunctionDeclarationNode& funcDecl, VnlcMetadataInfo metadataInfo = VnlcMetadataInfo::DEFAULT);
    void checkClassDeclaration(const VnlcClassDeclarationNode& classDecl, VnlcMetadataInfo metadataInfo = VnlcMetadataInfo::DEFAULT);
    void checkInterfaceDeclaration(const VnlcInterfaceDeclarationNode& interfaceDecl, VnlcMetadataInfo metadataInfo = VnlcMetadataInfo::DEFAULT);
    void checkEnumDeclaration(const VnlcEnumDeclarationNode& enumDecl, VnlcMetadataInfo metadataInfo = VnlcMetadataInfo::DEFAULT);
    void checkTypeAliasDeclaration(const VnlcTypeAliasDeclarationNode& typeAliasDecl, VnlcMetadataInfo metadataInfo = VnlcMetadataInfo::DEFAULT);

    void checkStatement(const VnlcStatementNode& statement);
    void checkExpression(const VnlcExpressionNode& expression);
    void checkType(const VnlcTypeNode& type);
    void checkConstExpression(const VnlcExpressionNode& expression);

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
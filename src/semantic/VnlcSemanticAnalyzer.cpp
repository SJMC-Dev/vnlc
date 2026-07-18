#include "VnlcSemanticAnalyzer.hpp"
#include <fmt/core.h>

VnlcSemanticAnalyzer::VnlcSemanticAnalyzer(const VnlcModuleNode& module) : module(module) {}

void VnlcSemanticAnalyzer::checkModule(const VnlcModuleNode& moduleNode) {
    context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::MODULE, nullptr));

    for (const auto& importDecl : moduleNode.getImportDeclarations()) {
        checkImport(*importDecl);
    }

    for (const auto& topIdentifierDecl : moduleNode.getTopIdentifierDeclarations()) {
        VnlcDeclarationNode* declNode = topIdentifierDecl.get();

        if (auto* varDecl = dynamic_cast<VnlcVariableDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::VARIABLE, VnlcSymbolOrigin::LOCAL, varDecl->getName(), varDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*varDecl, fmt::format("Redeclaration of symbol '{}'", varDecl->getName()));
            }
            checkVariableDeclaration(*varDecl);
        } else if (auto* funcDecl = dynamic_cast<VnlcFunctionDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::FUNCTION, VnlcSymbolOrigin::LOCAL, funcDecl->getName(), funcDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*funcDecl, fmt::format("Redeclaration of symbol '{}'", funcDecl->getName()));
            }
            checkFunctionDeclaration(*funcDecl);
        } else if (auto* classDecl = dynamic_cast<VnlcClassDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::CLASS, VnlcSymbolOrigin::LOCAL, classDecl->getName(), classDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*classDecl, fmt::format("Redeclaration of symbol '{}'", classDecl->getName()));
            }
            checkClassDeclaration(*classDecl);
        } else if (auto* interfaceDecl = dynamic_cast<VnlcInterfaceDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::INTERFACE, VnlcSymbolOrigin::LOCAL, interfaceDecl->getName(), interfaceDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*interfaceDecl, fmt::format("Redeclaration of symbol '{}'", interfaceDecl->getName()));
            }
            checkInterfaceDeclaration(*interfaceDecl);
        } else if (auto* enumDecl = dynamic_cast<VnlcEnumDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::ENUM, VnlcSymbolOrigin::LOCAL, enumDecl->getName(), enumDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*enumDecl, fmt::format("Redeclaration of symbol '{}'", enumDecl->getName()));
            }
            checkEnumDeclaration(*enumDecl);
        } else if (auto* typeAliasDecl = dynamic_cast<VnlcTypeAliasDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::TYPE_ALIAS, VnlcSymbolOrigin::LOCAL, typeAliasDecl->getAliasName(), typeAliasDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*typeAliasDecl, fmt::format("Redeclaration of symbol '{}'", typeAliasDecl->getAliasName()));
            }
            checkTypeAliasDeclaration(*typeAliasDecl);
        }
    }

    for (const auto& exportDecl : moduleNode.getExportDeclarations()) {
        checkExport(*exportDecl);
    }
}

VnlcSemanticAnalysisResult VnlcSemanticAnalyzer::analyze(const VnlcConfig& config) {
    checkModule(module);

    auto diagnostics = context.takeDiagnostics();
    return VnlcSemanticAnalysisResult(std::move(std::get<0>(diagnostics)), std::move(std::get<1>(diagnostics)), std::move(std::get<2>(diagnostics)));
}
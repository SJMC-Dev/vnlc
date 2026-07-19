#include "VnlcSemanticAnalyzer.hpp"
#include <fmt/core.h>

VnlcSemanticAnalyzer::VnlcSemanticAnalyzer(const VnlcModuleNode& module) : module(module) {}

bool VnlcSemanticAnalyzer::checkIdentifierName(std::string_view name, const VnlcDeclarationNode& declNode) {
    if (name.starts_with("__")) {
        context.reportWarning(declNode, fmt::format("Identifier '{}' starts with '__', which is reserved for internal use", name));
    }

    return true;
}

bool VnlcSemanticAnalyzer::checkIdentifierExpressionUse(const VnlcIdentifierExpressionNode& exprNode) {
    auto symbol = context.currentScope().lookup(exprNode.getName());
    if (symbol == std::nullopt) {
        context.reportError(exprNode, fmt::format("Use of undeclared identifier '{}'", exprNode.getName()));
        return false;
    } else if (!dynamic_cast<const VnlcVariableDeclarationNode*>(symbol.value()->getLocalDeclarationNode())) {
        context.reportError(exprNode, fmt::format("Identifier '{}' is not a variable", exprNode.getName()));
        return false;
    }

    return true;
}

bool VnlcSemanticAnalyzer::checkModule(const VnlcModuleNode& moduleNode) {
    bool success = true;
    context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::MODULE, nullptr));

    for (const auto& importDecl : moduleNode.getImportDeclarations()) {
        checkImport(*importDecl);
    }

    for (const auto& topIdentifierDecl : moduleNode.getTopIdentifierDeclarations()) {
        VnlcDeclarationNode* declNode = topIdentifierDecl.get();

        if (auto* varDecl = dynamic_cast<VnlcVariableDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::VARIABLE, VnlcSymbolOrigin::LOCAL, varDecl->getName(), varDecl->getName(), varDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*varDecl, fmt::format("Redeclaration of symbol '{}'", varDecl->getName()));
            }
        } else if (auto* funcDecl = dynamic_cast<VnlcFunctionDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::FUNCTION, VnlcSymbolOrigin::LOCAL, funcDecl->getUniqueName(), funcDecl->getName(), funcDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*funcDecl, fmt::format("Redeclaration of symbol '{}'", funcDecl->getName()));
            }
        } else if (auto* classDecl = dynamic_cast<VnlcClassDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::CLASS, VnlcSymbolOrigin::LOCAL, classDecl->getName(), classDecl->getName(), classDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*classDecl, fmt::format("Redeclaration of symbol '{}'", classDecl->getName()));
            }
        } else if (auto* interfaceDecl = dynamic_cast<VnlcInterfaceDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::INTERFACE, VnlcSymbolOrigin::LOCAL, interfaceDecl->getName(), interfaceDecl->getName(), interfaceDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*interfaceDecl, fmt::format("Redeclaration of symbol '{}'", interfaceDecl->getName()));
            }
        } else if (auto* enumDecl = dynamic_cast<VnlcEnumDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::ENUM, VnlcSymbolOrigin::LOCAL, enumDecl->getName(), enumDecl->getName(), enumDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*enumDecl, fmt::format("Redeclaration of symbol '{}'", enumDecl->getName()));
            }
        } else if (auto* typeAliasDecl = dynamic_cast<VnlcTypeAliasDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::TYPE_ALIAS, VnlcSymbolOrigin::LOCAL, typeAliasDecl->getAliasName(), typeAliasDecl->getAliasName(), typeAliasDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*typeAliasDecl, fmt::format("Redeclaration of symbol '{}'", typeAliasDecl->getAliasName()));
            }
        }
    }

    for (const auto& topIdentifierDecl : moduleNode.getTopIdentifierDeclarations()) {
        VnlcDeclarationNode* declNode = topIdentifierDecl.get();

        if (auto* varDecl = dynamic_cast<VnlcVariableDeclarationNode*>(declNode)) {
            checkVariableDeclaration(*varDecl);
        } else if (auto* funcDecl = dynamic_cast<VnlcFunctionDeclarationNode*>(declNode)) {
            checkFunctionDeclaration(*funcDecl);
        } else if (auto* classDecl = dynamic_cast<VnlcClassDeclarationNode*>(declNode)) {
            checkClassDeclaration(*classDecl);
        } else if (auto* interfaceDecl = dynamic_cast<VnlcInterfaceDeclarationNode*>(declNode)) {
            checkInterfaceDeclaration(*interfaceDecl);
        } else if (auto* enumDecl = dynamic_cast<VnlcEnumDeclarationNode*>(declNode)) {
            checkEnumDeclaration(*enumDecl);
        } else if (auto* typeAliasDecl = dynamic_cast<VnlcTypeAliasDeclarationNode*>(declNode)) {
            checkTypeAliasDeclaration(*typeAliasDecl);
        }
    }

    for (const auto& exportDecl : moduleNode.getExportDeclarations()) {
        checkExport(*exportDecl);
    }

    return success;
}

bool VnlcSemanticAnalyzer::checkImport(const VnlcImportDeclarationNode& importDecl) {
    // TODO: Implement import checking logic
    return true;
}

bool VnlcSemanticAnalyzer::checkExport(const VnlcExportDeclarationNode& exportDecl) {
    // TODO: Implement export checking logic
    return true;
}

bool VnlcSemanticAnalyzer::checkVariableDeclaration(const VnlcVariableDeclarationNode& varDecl) {
    bool success = true;
    if (varDecl.getType() == VnlcVariableDeclarationType::CONST) {
        const VnlcExpressionNode* initializer = &varDecl.getInitializer();
        if (const auto* stringLiteral = dynamic_cast<const VnlcStringLiteralExpressionNode*>(initializer)) {
            if (stringLiteral->getType() == VnlcStringLiteralExpressionType::FORMAT_STRING) {
                context.reportError(varDecl, "Const variables cannot be initialized with format strings");
                success = false;
            } else if (!dynamic_cast<const VnlcSimpleLiteralExpressionNode*>(initializer)) {
                context.reportError(varDecl, "Const variables must be initialized with simple literals or non-format strings");
                success = false;
            }
        }
    }

    success &= checkIdentifierName(varDecl.getName(), varDecl);
    success &= checkExpression(varDecl.getInitializer());

    // TODO: Implement type checking and inference

    return success;
}

VnlcSemanticAnalysisResult VnlcSemanticAnalyzer::analyze(const VnlcConfig& config) {
    checkModule(module);

    auto diagnostics = context.takeDiagnostics();
    return VnlcSemanticAnalysisResult(std::move(std::get<0>(diagnostics)), std::move(std::get<1>(diagnostics)), std::move(std::get<2>(diagnostics)));
}
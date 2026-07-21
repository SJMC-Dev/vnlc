#include "VnlcSemanticAnalyzer.hpp"
#include "../ast/expression/VnlcIdentifierExpressionNode.hpp"
#include "../ast/expression/VnlcSimpleLiteralExpressionNode.hpp"
#include "../ast/expression/VnlcStringLiteralExpressionNode.hpp"
#include <fmt/core.h>
#include <string_view>

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
    } else if (!(dynamic_cast<const VnlcValueDeclarationNode*>(symbol.value()->getLocalDeclarationNode()) ||
                 dynamic_cast<const VnlcFunctionDeclarationNode*>(symbol.value()->getLocalDeclarationNode()))) {
        context.reportError(exprNode, fmt::format("Identifier '{}' is not a variable or function", exprNode.getName()));
        return false;
    }

    return true;
}

bool VnlcSemanticAnalyzer::checkModule(const VnlcModuleNode& moduleNode) {
    bool success = true;
    context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::MODULE, nullptr));

    for (const auto& importDecl : moduleNode.getImportDeclarations()) {
        success &= checkImport(*importDecl);
    }

    for (const auto& topIdentifierDecl : moduleNode.getTopIdentifierDeclarations()) {
        VnlcDeclarationNode* declNode = topIdentifierDecl.get();

        if (auto* varDecl = dynamic_cast<VnlcValueDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::VARIABLE, VnlcSymbolOrigin::LOCAL, varDecl->getName(), varDecl->getName(), varDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*varDecl, fmt::format("Redeclaration of symbol '{}'", varDecl->getName()));
                success = false;
            }
        } else if (auto* funcDecl = dynamic_cast<VnlcFunctionDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::FUNCTION, VnlcSymbolOrigin::LOCAL, funcDecl->getUniqueName(), funcDecl->getName(), funcDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*funcDecl, fmt::format("Redeclaration of symbol '{}'", funcDecl->getName()));
                success = false;
            }
        } else if (auto* classDecl = dynamic_cast<VnlcClassDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::CLASS, VnlcSymbolOrigin::LOCAL, classDecl->getName(), classDecl->getName(), classDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*classDecl, fmt::format("Redeclaration of symbol '{}'", classDecl->getName()));
                success = false;
            }
        } else if (auto* interfaceDecl = dynamic_cast<VnlcInterfaceDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::INTERFACE, VnlcSymbolOrigin::LOCAL, interfaceDecl->getName(), interfaceDecl->getName(), interfaceDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*interfaceDecl, fmt::format("Redeclaration of symbol '{}'", interfaceDecl->getName()));
                success = false;
            }
        } else if (auto* enumDecl = dynamic_cast<VnlcEnumDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::ENUM, VnlcSymbolOrigin::LOCAL, enumDecl->getName(), enumDecl->getName(), enumDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*enumDecl, fmt::format("Redeclaration of symbol '{}'", enumDecl->getName()));
                success = false;
            }
        } else if (auto* typeAliasDecl = dynamic_cast<VnlcTypeAliasDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::TYPE_ALIAS, VnlcSymbolOrigin::LOCAL, typeAliasDecl->getAliasName(), typeAliasDecl->getAliasName(), typeAliasDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*typeAliasDecl, fmt::format("Redeclaration of symbol '{}'", typeAliasDecl->getAliasName()));
                success = false;
            }
        }
    }

    for (const auto& topIdentifierDecl : moduleNode.getTopIdentifierDeclarations()) {
        VnlcDeclarationNode* declNode = topIdentifierDecl.get();

        if (auto* varDecl = dynamic_cast<VnlcValueDeclarationNode*>(declNode)) {
            success &= checkVariableDeclaration(*varDecl);
        } else if (auto* funcDecl = dynamic_cast<VnlcFunctionDeclarationNode*>(declNode)) {
            success &= checkFunctionDeclaration(*funcDecl);
        } else if (auto* classDecl = dynamic_cast<VnlcClassDeclarationNode*>(declNode)) {
            success &= checkClassDeclaration(*classDecl);
        } else if (auto* interfaceDecl = dynamic_cast<VnlcInterfaceDeclarationNode*>(declNode)) {
            success &= checkInterfaceDeclaration(*interfaceDecl);
        } else if (auto* enumDecl = dynamic_cast<VnlcEnumDeclarationNode*>(declNode)) {
            success &= checkEnumDeclaration(*enumDecl);
        } else if (auto* typeAliasDecl = dynamic_cast<VnlcTypeAliasDeclarationNode*>(declNode)) {
            success &= checkTypeAliasDeclaration(*typeAliasDecl);
        }
    }

    for (const auto& exportDecl : moduleNode.getExportDeclarations()) {
        success &= checkExport(*exportDecl);
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

bool VnlcSemanticAnalyzer::checkVariableDeclaration(const VnlcValueDeclarationNode& varDecl) {
    bool success = true;
    if (varDecl.getKind() == VnlcValueDeclarationType::Kind::CONST) {
        const VnlcExpressionNode* initializer = varDecl.getInitializer().value().get();
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
    success &= checkExpression(*varDecl.getInitializer().value());

    // TODO: Implement type checking and inference

    return success;
}

bool VnlcSemanticAnalyzer::checkFunctionDeclaration(const VnlcFunctionDeclarationNode& funcDecl) {
    bool success = true;
    success &= checkIdentifierName(funcDecl.getName(), funcDecl);

    context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::FUNCTION, &context.currentScope()));
    for (const auto& param : funcDecl.getParameters()) {
        VnlcSymbol paramSymbol(VnlcSymbolKind::VARIABLE, VnlcSymbolOrigin::LOCAL, param->getName(), param->getName(), param.get());
        if (!context.currentScope().declare(std::move(paramSymbol))) {
            context.reportError(*param, fmt::format("Redeclaration of parameter '{}'", param->getName()));
            success = false;
        }
        success &= checkIdentifierName(param->getName(), *param);
        success &= checkType(param->getTypeAnnotation().value()->getTypeNode());
    }

    // TODO: Implement return type checking and inference

    return success;
}

VnlcSemanticAnalysisResult VnlcSemanticAnalyzer::analyze(const VnlcConfig& config) {
    checkModule(module);

    auto diagnostics = context.takeDiagnostics();
    return VnlcSemanticAnalysisResult(std::move(std::get<0>(diagnostics)), std::move(std::get<1>(diagnostics)), std::move(std::get<2>(diagnostics)));
}
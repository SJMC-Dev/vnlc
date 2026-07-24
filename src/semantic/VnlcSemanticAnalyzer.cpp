#include "VnlcSemanticAnalyzer.hpp"
#include "../ast/expression/VnlcIdentifierExpressionNode.hpp"
#include "../ast/expression/VnlcSimpleLiteralExpressionNode.hpp"
#include "../ast/expression/VnlcStringLiteralExpressionNode.hpp"
#include <fmt/core.h>
#include <string_view>

VnlcSemanticAnalyzer::VnlcSemanticAnalyzer(const VnlcModuleNode& module) : module(module) {}

void VnlcSemanticAnalyzer::checkIdentifierName(std::string_view name, const VnlcDeclarationNode& declNode) {
    if (name.starts_with("__")) {
        context.reportWarning(declNode, fmt::format("Identifier '{}' starts with '__', which is reserved for internal use", name));
    }
}

void VnlcSemanticAnalyzer::checkIdentifierExpressionUse(const VnlcIdentifierExpressionNode& exprNode) {
    auto symbol = context.currentScope().lookup(exprNode.getName());
    if (!symbol.has_value()) {
        context.reportError(exprNode, fmt::format("Use of undeclared identifier '{}'", exprNode.getName()));
    } else if (!(dynamic_cast<const VnlcValueDeclarationNode*>(symbol.value()->getLocalDeclarationNode()) ||
                 dynamic_cast<const VnlcFunctionDeclarationNode*>(symbol.value()->getLocalDeclarationNode()))) {
        context.reportError(exprNode, fmt::format("Identifier '{}' is not a variable or function", exprNode.getName()));
    }
}

void VnlcSemanticAnalyzer::checkMetadata(const std::vector<VnlcDeclarationItem::MetadataTerm>& metadataTerms, const VnlcDeclarationNode& declNode) {
    // TODO: Implement metadata checking
}

void VnlcSemanticAnalyzer::checkModule(const VnlcModuleNode& moduleNode, const VnlcConfig& config) {
    context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::MODULE, nullptr));

    for (const auto& importDecl : moduleNode.getImportDeclarations()) {
        checkImport(*importDecl, config);
    }

    for (const auto& topIdentifierDecl : moduleNode.getTopIdentifierDeclarations()) {
        VnlcDeclarationNode* declNode = topIdentifierDecl.get();

        if (auto* varDecl = dynamic_cast<VnlcValueDeclarationNode*>(declNode)) {
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

        if (auto* varDecl = dynamic_cast<VnlcValueDeclarationNode*>(declNode)) {
            if (varDecl->getKind() == VnlcValueDeclarationType::Kind::VAR || varDecl->getKind() == VnlcValueDeclarationType::Kind::LET ||
                varDecl->getKind() == VnlcValueDeclarationType::Kind::CONST) {
                checkValueDeclaration(*varDecl);
            } else {
                context.reportError(*varDecl, fmt::format("Top-level value declaration must be a variable"));
            }
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

    context.popScope();
}

void VnlcSemanticAnalyzer::checkImport(const VnlcImportDeclarationNode& importDecl, const VnlcConfig& config) {
    // TODO: Implement import checking logic
}

void VnlcSemanticAnalyzer::checkExport(const VnlcExportDeclarationNode& exportDecl) {
    // TODO: Implement export checking logic
}

void VnlcSemanticAnalyzer::checkValueDeclaration(const VnlcValueDeclarationNode& varDecl) {
    checkIdentifierName(varDecl.getName(), varDecl);
    auto kind = varDecl.getKind();

    if (kind == VnlcValueDeclarationType::Kind::CONST) {
        if (!varDecl.getInitializer().has_value()) {
            context.reportError(varDecl, "Const variables must be initialized");
        } else if (const auto* stringLiteral = dynamic_cast<const VnlcStringLiteralExpressionNode*>(varDecl.getInitializer().value().get())) {
            if (stringLiteral->getType() == VnlcStringLiteralExpressionType::FORMAT_STRING) {
                context.reportError(varDecl, "Const variables can't be initialized with a format string");
            }
        } else if (!dynamic_cast<const VnlcSimpleLiteralExpressionNode*>(varDecl.getInitializer().value().get())) {
            context.reportError(varDecl, "Const variables must be initialized with a simple literal or a non-format string literal expression");
        }
    } else if (kind == VnlcValueDeclarationType::Kind::STATIC_PROPERTY) {
        if (!varDecl.getInitializer().has_value()) {
            context.reportError(varDecl, "Static properties must be initialized");
        }
    }

    if (varDecl.getInitializer().has_value()) {
        checkExpression(*varDecl.getInitializer().value());
    }

    // TODO: Implement type checking and inference
}

void VnlcSemanticAnalyzer::checkFunctionDeclaration(const VnlcFunctionDeclarationNode& funcDecl) {
    checkIdentifierName(funcDecl.getName(), funcDecl);

    context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::FUNCTION, &context.currentScope()));
    for (const auto& param : funcDecl.getParameters()) {
        VnlcSymbol paramSymbol(VnlcSymbolKind::PARAMETER, VnlcSymbolOrigin::LOCAL, param->getName(), param->getName(), param.get());
        if (!context.currentScope().declare(std::move(paramSymbol))) {
            context.reportError(*param, fmt::format("Redeclaration of parameter '{}'", param->getName()));
        }

        checkValueDeclaration(*param);
    }

    if (funcDecl.getKind() == VnlcFunctionDeclarationType::Kind::REGULAR && funcDecl.getContext() != VnlcFunctionDeclarationType::Context::INTERFACE) {
        if (funcDecl.getBody().has_value()) {
            checkStatement(*funcDecl.getBody().value());
        } else {
            context.reportError(funcDecl, "Regular functions must have a body");
        }
    }

    // TODO: Implement return type checking and inference

    context.popScope();
}

void VnlcSemanticAnalyzer::checkClassDeclaration(const VnlcClassDeclarationNode& classDecl) {
    checkIdentifierName(classDecl.getName(), classDecl);

    context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::CLASS, &context.currentScope()));
    for (const auto& member : classDecl.getMemberDeclarations()) {
        if (auto* varDecl = dynamic_cast<VnlcValueDeclarationNode*>(member.get())) {
            checkValueDeclaration(*varDecl);
        } else if (auto* funcDecl = dynamic_cast<VnlcFunctionDeclarationNode*>(member.get())) {
            checkFunctionDeclaration(*funcDecl);
        } else {
            context.reportError(*member, "Invalid class member declaration");
        }
    }

    context.popScope();
}

VnlcSemanticAnalysisResult VnlcSemanticAnalyzer::analyze(const VnlcConfig& config) {
    checkModule(module, config);

    auto diagnostics = context.takeDiagnostics();
    return VnlcSemanticAnalysisResult(std::move(std::get<0>(diagnostics)), std::move(std::get<1>(diagnostics)), std::move(std::get<2>(diagnostics)));
}
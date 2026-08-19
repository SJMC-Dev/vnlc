#include "VnlcSemanticAnalyzer.hpp"
#include "../ast/expression/VnlcIdentifierExpressionNode.hpp"
#include "../ast/expression/VnlcSimpleLiteralExpressionNode.hpp"
#include "../ast/expression/VnlcStringLiteralExpressionNode.hpp"
#include "../ast/statement/VnlcBlockStatementNode.hpp"
#include "../ast/statement/VnlcBreakStatementNode.hpp"
#include "../ast/statement/VnlcContinueStatementNode.hpp"
#include "../ast/statement/VnlcExpressionStatementNode.hpp"
#include "../ast/statement/VnlcForStatementNode.hpp"
#include "../ast/statement/VnlcIfStatementNode.hpp"
#include "../ast/statement/VnlcReturnStatementNode.hpp"
#include "../ast/statement/VnlcSwitchStatementNode.hpp"
#include "../ast/statement/VnlcVariableDeclarationStatementNode.hpp"
#include "../ast/statement/VnlcWhileStatementNode.hpp"
#include "symbol/VnlcSymbolKind.hpp"
#include "symbol/VnlcSymbolOrigin.hpp"
#include <fmt/core.h>
#include <string_view>

VnlcSemanticAnalyzer::VnlcSemanticAnalyzer(const VnlcModuleNode& module) : module(module) {}

void VnlcSemanticAnalyzer::checkIdentifierName(std::string_view name, const VnlcDeclarationNode& declNode, VnlcMetadataInfo metadataInfo) {
    if (name.starts_with("__")) {
        context.reportWarning(declNode, fmt::format("Identifier '{}' starts with '__', which is reserved for internal use", name));
    }
}

void VnlcSemanticAnalyzer::checkIdentifierExpressionUse(const VnlcIdentifierExpressionNode& exprNode, VnlcMetadataInfo metadataInfo) {
    auto symbol = context.currentScope().lookup(exprNode.getName());
    if (!symbol.has_value()) {
        context.reportError(exprNode, fmt::format("Use of undeclared identifier '{}'", exprNode.getName()));
    } else if (!(dynamic_cast<const VnlcValueDeclarationNode*>(symbol.value()->getLocalDeclarationNode()) ||
                 dynamic_cast<const VnlcFunctionDeclarationNode*>(symbol.value()->getLocalDeclarationNode()))) {
        context.reportError(exprNode, fmt::format("Identifier '{}' is not a variable or function", exprNode.getName()));
    }
}

VnlcMetadataInfo VnlcSemanticAnalyzer::checkMetadata(const std::vector<VnlcDeclarationItem::MetadataTerm>& metadataTerms, const VnlcDeclarationNode& declNode) {
    bool noWarnings = false;
    bool deprecated = false;

    for (const auto& term : metadataTerms) {
        if (term.key == "nowarnings") {
            noWarnings = true;
        } else if (term.key == "deprecated") {
            deprecated = true;
        } else {
            context.reportError(declNode, fmt::format("Unknown metadata term '{}'", term.key));
        }
    }

    return VnlcMetadataInfo{
        noWarnings,
        deprecated,
    };
}

void VnlcSemanticAnalyzer::checkModule(const VnlcModuleNode& moduleNode, const VnlcConfig& config) {
    context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::MODULE, nullptr));

    for (const auto& importDecl : moduleNode.getImportDeclarations()) {
        checkImport(*importDecl, config);
    }

    for (const auto& topIdentifierDecl : moduleNode.getTopIdentifierDeclarations()) {
        VnlcDeclarationNode* declNode = topIdentifierDecl.get();

        if (auto* varDecl = dynamic_cast<VnlcValueDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::VARIABLE, VnlcSymbolOrigin::LOCAL, varDecl->getName(), varDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*varDecl, fmt::format("Redeclaration of symbol '{}'", varDecl->getName()));
            }
        } else if (auto* funcDecl = dynamic_cast<VnlcFunctionDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::FUNCTION, VnlcSymbolOrigin::LOCAL, funcDecl->getName(), funcDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*funcDecl, fmt::format("Redeclaration of symbol '{}'", funcDecl->getName()));
            }
        } else if (auto* classDecl = dynamic_cast<VnlcClassDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::CLASS, VnlcSymbolOrigin::LOCAL, classDecl->getName(), classDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*classDecl, fmt::format("Redeclaration of symbol '{}'", classDecl->getName()));
            }
        } else if (auto* interfaceDecl = dynamic_cast<VnlcInterfaceDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::INTERFACE, VnlcSymbolOrigin::LOCAL, interfaceDecl->getName(), interfaceDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*interfaceDecl, fmt::format("Redeclaration of symbol '{}'", interfaceDecl->getName()));
            }
        } else if (auto* enumDecl = dynamic_cast<VnlcEnumDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::ENUM, VnlcSymbolOrigin::LOCAL, enumDecl->getName(), enumDecl);
            if (!context.currentScope().declare(std::move(symbol))) {
                context.reportError(*enumDecl, fmt::format("Redeclaration of symbol '{}'", enumDecl->getName()));
            }
        } else if (auto* typeAliasDecl = dynamic_cast<VnlcTypeAliasDeclarationNode*>(declNode)) {
            VnlcSymbol symbol(VnlcSymbolKind::TYPE_ALIAS, VnlcSymbolOrigin::LOCAL, typeAliasDecl->getAliasName(), typeAliasDecl);
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
    for (auto& item : exportDecl.getNamesListWithAliases()) {
        if (!context.currentScope().lookup(item.name).has_value()) {
            context.reportError(exportDecl, fmt::format("Undefined symbol {}", item.name));
        }

        if (item.alias.has_value()) {
            VnlcSymbol aliasSymbol(VnlcSymbolKind::EXPORT_ALIAS, VnlcSymbolOrigin::LOCAL, item.alias.value(), &exportDecl);

            if (!context.currentScope().declare(std::move(aliasSymbol))) {
                context.reportError(exportDecl, fmt::format("Redeclaration of symbol {}", item.alias.value()));
            }
        }
    }
}

void VnlcSemanticAnalyzer::checkValueDeclaration(const VnlcValueDeclarationNode& varDecl, VnlcMetadataInfo metadataInfo) {
    checkIdentifierName(varDecl.getName(), varDecl, metadataInfo);
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

void VnlcSemanticAnalyzer::checkFunctionDeclaration(const VnlcFunctionDeclarationNode& funcDecl, VnlcMetadataInfo metadataInfo) {
    checkIdentifierName(funcDecl.getName(), funcDecl, metadataInfo);

    context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::FUNCTION, &context.currentScope()));
    for (const auto& param : funcDecl.getParameters()) {
        VnlcSymbol paramSymbol(VnlcSymbolKind::PARAMETER, VnlcSymbolOrigin::LOCAL, param->getName(), param.get());
        if (!context.currentScope().declare(std::move(paramSymbol))) {
            context.reportError(*param, fmt::format("Redeclaration of parameter '{}'", param->getName()));
        }
    }

    for (const auto& param : funcDecl.getParameters()) {
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

void VnlcSemanticAnalyzer::checkClassDeclaration(const VnlcClassDeclarationNode& classDecl, VnlcMetadataInfo metadataInfo) {
    checkIdentifierName(classDecl.getName(), classDecl, metadataInfo);

    context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::CLASS, &context.currentScope()));

    for (const auto& member : classDecl.getMemberDeclarations()) {
        if (auto* varDecl = dynamic_cast<VnlcValueDeclarationNode*>(member.get())) {
            VnlcSymbol memberSymbol(VnlcSymbolKind::PROPERTY, VnlcSymbolOrigin::LOCAL, varDecl->getName(), varDecl);
            if (!context.currentScope().declare(std::move(memberSymbol))) {
                context.reportError(*varDecl, fmt::format("Redeclaration of class member '{}'", varDecl->getName()));
            }
        } else if (auto* funcDecl = dynamic_cast<VnlcFunctionDeclarationNode*>(member.get())) {
            VnlcSymbol memberSymbol(VnlcSymbolKind::METHOD, VnlcSymbolOrigin::LOCAL, funcDecl->getName(), funcDecl);
            if (!context.currentScope().declare(std::move(memberSymbol))) {
                context.reportError(*funcDecl, fmt::format("Redeclaration of class member '{}'", funcDecl->getName()));
            }
        } else {
            context.reportError(*member, "Invalid class member declaration");
        }
    }

    for (const auto& genericParamName : classDecl.getGenericParameterNames()) {
        VnlcSymbol genericParamSymbol(VnlcSymbolKind::GENERIC_PARAMETER, VnlcSymbolOrigin::LOCAL, genericParamName, nullptr);
        if (!context.currentScope().declare(std::move(genericParamSymbol))) {
            context.reportError(classDecl, fmt::format("Redeclaration of generic parameter '{}'", genericParamName));
        }
    }

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

void VnlcSemanticAnalyzer::checkInterfaceDeclaration(const VnlcInterfaceDeclarationNode& interfaceDecl, VnlcMetadataInfo metadataInfo) {
    checkIdentifierName(interfaceDecl.getName(), interfaceDecl, metadataInfo);

    context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::INTERFACE, &context.currentScope()));

    for (const auto& member : interfaceDecl.getMethodDeclarations()) {
        VnlcSymbol memberSymbol(VnlcSymbolKind::METHOD, VnlcSymbolOrigin::LOCAL, member->getName(), member.get());
        if (!context.currentScope().declare(std::move(memberSymbol))) {
            context.reportError(*member, fmt::format("Redeclaration of interface method '{}'", member->getName()));
        }
    }

    for (const auto& genericParamName : interfaceDecl.getGenericParameterNames()) {
        VnlcSymbol genericParamSymbol(VnlcSymbolKind::GENERIC_PARAMETER, VnlcSymbolOrigin::LOCAL, genericParamName, nullptr);
        if (!context.currentScope().declare(std::move(genericParamSymbol))) {
            context.reportError(interfaceDecl, fmt::format("Redeclaration of generic parameter '{}'", genericParamName));
        }
    }

    for (const auto& member : interfaceDecl.getMethodDeclarations()) {
        if (auto* funcDecl = dynamic_cast<VnlcFunctionDeclarationNode*>(member.get())) {
            checkFunctionDeclaration(*funcDecl);
        } else {
            context.reportError(*member, "Invalid interface member declaration");
        }
    }

    context.popScope();
}

void VnlcSemanticAnalyzer::checkEnumDeclaration(const VnlcEnumDeclarationNode& enumDecl, VnlcMetadataInfo metadataInfo) {
    checkIdentifierName(enumDecl.getName(), enumDecl, metadataInfo);

    context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::ENUM, &context.currentScope()));

    for (const auto& member : enumDecl.getMemberDeclarations()) {
        VnlcSymbol memberSymbol(VnlcSymbolKind::ENUM_MEMBER, VnlcSymbolOrigin::LOCAL, member->getName(), member.get());
        if (!context.currentScope().declare(std::move(memberSymbol))) {
            context.reportError(*member, fmt::format("Redeclaration of enum member '{}'", member->getName()));
        }
    }

    for (const auto& genericParamName : enumDecl.getGenericParameterNames()) {
        VnlcSymbol genericParamSymbol(VnlcSymbolKind::GENERIC_PARAMETER, VnlcSymbolOrigin::LOCAL, genericParamName, nullptr);
        if (!context.currentScope().declare(std::move(genericParamSymbol))) {
            context.reportError(enumDecl, fmt::format("Redeclaration of generic parameter '{}'", genericParamName));
        }
    }

    for (const auto& member : enumDecl.getMemberDeclarations()) {
        context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::ENUM_MEMBER, &context.currentScope()));

        for (auto& associatedValue : member->getAssociatedValues()) {
            VnlcSymbol associatedValueSymbol(VnlcSymbolKind::PROPERTY, VnlcSymbolOrigin::LOCAL, associatedValue->getName(), associatedValue.get());
            if (!context.currentScope().declare(std::move(associatedValueSymbol))) {
                context.reportError(*associatedValue, fmt::format("Redeclaration of enum member associated value '{}'", associatedValue->getName()));
            }
        }

        for (auto& associatedValue : member->getAssociatedValues()) {
            checkValueDeclaration(*associatedValue);
        }

        context.popScope();
    }

    context.popScope();
}

void VnlcSemanticAnalyzer::checkTypeAliasDeclaration(const VnlcTypeAliasDeclarationNode& typeAliasDecl, VnlcMetadataInfo metadataInfo) {
    checkIdentifierName(typeAliasDecl.getAliasName(), typeAliasDecl, metadataInfo);

    context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::TYPE_ALIAS, &context.currentScope()));

    for (const auto& genericParamName : typeAliasDecl.getGenericParameterNames()) {
        VnlcSymbol genericParamSymbol(VnlcSymbolKind::GENERIC_PARAMETER, VnlcSymbolOrigin::LOCAL, genericParamName, nullptr);
        if (!context.currentScope().declare(std::move(genericParamSymbol))) {
            context.reportError(typeAliasDecl, fmt::format("Redeclaration of generic parameter '{}'", genericParamName));
        }
    }

    checkType(typeAliasDecl.getOriginalType());

    context.popScope();
}

void VnlcSemanticAnalyzer::checkStatement(const VnlcStatementNode& statement) {
    if (auto* stmt = dynamic_cast<const VnlcBlockStatementNode*>(&statement)) {
        context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::BLOCK, &context.currentScope()));

        for (const auto& child : stmt->getStatements()) {
            checkStatement(*child);
        }

        context.popScope();
    } else if (auto* stmt = dynamic_cast<const VnlcBreakStatementNode*>(&statement)) {
        if (!context.currentLoop()) {
            context.reportError(*stmt, "Break statement not within a loop");
        }

        auto label = stmt->getLabel();
        if (label.has_value()) {
            auto labelSymbol = context.currentScope().lookup(label.value());
            if (!labelSymbol.has_value()) {
                context.reportError(*stmt, fmt::format("Label '{}' does not exist", label.value()));
            } else if (labelSymbol.value()->getKind() != VnlcSymbolKind::LOOP_LABEL) {
                context.reportError(*stmt, fmt::format("Identifier '{}' is not a loop label", label.value()));
            }
        }
    } else if (auto* stmt = dynamic_cast<const VnlcContinueStatementNode*>(&statement)) {
        if (!context.currentLoop()) {
            context.reportError(*stmt, "Continue statement not within a loop");
        }

        auto label = stmt->getLabel();
        if (label.has_value()) {
            auto labelSymbol = context.currentScope().lookup(label.value());
            if (!labelSymbol.has_value()) {
                context.reportError(*stmt, fmt::format("Label '{}' does not exist", label.value()));
            } else if (labelSymbol.value()->getKind() != VnlcSymbolKind::LOOP_LABEL) {
                context.reportError(*stmt, fmt::format("Identifier '{}' is not a loop label", label.value()));
            }
        }
    } else if (auto* stmt = dynamic_cast<const VnlcExpressionStatementNode*>(&statement)) {
        checkExpression(stmt->getExpression());
    } else if (auto* stmt = dynamic_cast<const VnlcForStatementNode*>(&statement)) {
        context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::LOOP, &context.currentScope()));

        auto label = stmt->getLabel();
        if (label.has_value()) {
            VnlcSymbol labelSymbol(VnlcSymbolKind::LOOP_LABEL, VnlcSymbolOrigin::LOCAL, label.value(), nullptr);

            if (!context.currentScope().declare(std::move(labelSymbol))) {
                context.reportError(*stmt, fmt::format("Redeclaration of identifier '{}'", label.value()));
            }
        }

        checkValueDeclaration(stmt->getLoopVariable());
        checkExpression(stmt->getIterableExpression());
        checkStatement(stmt->getBody());

        context.popScope();
    } else if (auto* stmt = dynamic_cast<const VnlcIfStatementNode*>(&statement)) {
        checkExpression(stmt->getCondition());
        checkStatement(stmt->getThenBranch());
        if (stmt->getElseBranch().has_value()) {
            checkStatement(*stmt->getElseBranch().value());
        }
    } else if (auto* stmt = dynamic_cast<const VnlcReturnStatementNode*>(&statement)) {
        if (stmt->getReturnValue().has_value()) {
            checkExpression(*stmt->getReturnValue().value());
        }
    } else if (auto* stmt = dynamic_cast<const VnlcSwitchStatementNode*>(&statement)) {
        context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::SWITCH, &context.currentScope()));

        checkExpression(stmt->getSwitchExpression());

        if (stmt->getSwitchType() == VnlcSwitchStatementType::LITERAL_MATCH) {
            for (const auto& item : stmt->getLiteralMatchItems()) {
                checkStatement(*item.body);
            }
        } else if (stmt->getSwitchType() == VnlcSwitchStatementType::TYPE_MATCH) {
            for (const auto& item : stmt->getTypeMatchItems()) {
                checkType(*item.type);
                checkStatement(*item.body);
            }
        }

        if (stmt->getDefaultCaseBody().has_value()) {
            checkStatement(*stmt->getDefaultCaseBody().value());
        }

        context.popScope();
    } else if (auto* stmt = dynamic_cast<const VnlcVariableDeclarationStatementNode*>(&statement)) {
        checkValueDeclaration(stmt->getVariableDeclaration());
    } else if (auto* stmt = dynamic_cast<const VnlcWhileStatementNode*>(&statement)) {
        context.pushScope(std::make_unique<VnlcScope>(VnlcScopeKind::LOOP, &context.currentScope()));

        auto label = stmt->getLabel();
        if (label.has_value()) {
            VnlcSymbol labelSymbol(VnlcSymbolKind::LOOP_LABEL, VnlcSymbolOrigin::LOCAL, label.value(), nullptr);

            if (!context.currentScope().declare(std::move(labelSymbol))) {
                context.reportError(*stmt, fmt::format("Redeclaration of identifier '{}'", label.value()));
            }
        }

        checkExpression(stmt->getCondition());
        checkStatement(stmt->getBody());

        context.popScope();
    } else {
        context.reportError(statement, "Unknown statement type");
    }
}

VnlcSemanticAnalysisResult VnlcSemanticAnalyzer::analyze(const VnlcConfig& config) {
    checkModule(module, config);

    auto diagnostics = context.takeDiagnostics();
    return VnlcSemanticAnalysisResult(std::move(std::get<0>(diagnostics)), std::move(std::get<1>(diagnostics)), std::move(std::get<2>(diagnostics)));
}
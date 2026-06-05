#pragma once

#ifndef VNLC_SEMANTIC_CONTEXT_HPP
#define VNLC_SEMANTIC_CONTEXT_HPP

#include "../ast/declaration/VnlcClassDeclarationNode.hpp"
#include "../ast/declaration/VnlcEnumDeclarationNode.hpp"
#include "../ast/declaration/VnlcFunctionDeclarationNode.hpp"
#include "../ast/declaration/VnlcInterfaceDeclarationNode.hpp"
#include "../diagnostic/VnlcDiagnostic.hpp"
#include "VnlcScope.hpp"
#include <memory>
#include <string_view>
#include <vector>

class VnlcSemanticContext {
private:
    std::vector<VnlcDiagnostic> errors;
    std::vector<VnlcDiagnostic> warnings;
    std::vector<VnlcDiagnostic> notes;

    std::vector<std::unique_ptr<VnlcScope>> scopeStack;

    const VnlcFunctionDeclarationNode* currentFunction = nullptr;
    const VnlcClassDeclarationNode* currentClass = nullptr;
    const VnlcInterfaceDeclarationNode* currentInterface = nullptr;
    const VnlcEnumDeclarationNode* currentEnum = nullptr;

    unsigned int loopDepth = 0;
    unsigned int switchDepth = 0;

public:
    VnlcSemanticContext() = default;
    VnlcSemanticContext(const VnlcSemanticContext&) = delete;
    VnlcSemanticContext& operator=(const VnlcSemanticContext&) = delete;
    VnlcSemanticContext(VnlcSemanticContext&&) noexcept = default;
    VnlcSemanticContext& operator=(VnlcSemanticContext&&) noexcept = default;

    void reportError(const VnlcAstNode& node, std::string_view message);
    void reportWarning(const VnlcAstNode& node, std::string_view message);
    void reportNote(const VnlcAstNode& node, std::string_view message);

    void pushScope(std::unique_ptr<VnlcScope>&& scope);
    void popScope();

    [[nodiscard]] VnlcScope& currentScope();
};

#endif // VNLC_SEMANTIC_CONTEXT_HPP
#ifndef VNLC_SEMANTIC_CONTEXT_HPP
#define VNLC_SEMANTIC_CONTEXT_HPP

#include "../diagnostic/VnlcDiagnostic.hpp"
#include "scope/VnlcScope.hpp"
#include <memory>
#include <string_view>
#include <tuple>
#include <vector>

class VnlcSemanticContext {
private:
    std::vector<VnlcDiagnostic> errors;
    std::vector<VnlcDiagnostic> warnings;
    std::vector<VnlcDiagnostic> notes;

    std::vector<std::unique_ptr<VnlcScope>> scopeStack;

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

    [[nodiscard]] const VnlcScope* currentModule();
    [[nodiscard]] const VnlcScope* currentFunction();
    [[nodiscard]] const VnlcScope* currentClass();
    [[nodiscard]] const VnlcScope* currentInterface();
    [[nodiscard]] const VnlcScope* currentEnum();
    [[nodiscard]] const VnlcScope* currentBlock();

    [[nodiscard]] const std::vector<VnlcDiagnostic>& getErrors() const noexcept;
    [[nodiscard]] const std::vector<VnlcDiagnostic>& getWarnings() const noexcept;
    [[nodiscard]] const std::vector<VnlcDiagnostic>& getNotes() const noexcept;

    [[nodiscard]] unsigned int getLoopDepth() const noexcept;
    [[nodiscard]] unsigned int getSwitchDepth() const noexcept;

    [[nodiscard]] std::tuple<std::vector<VnlcDiagnostic>, std::vector<VnlcDiagnostic>, std::vector<VnlcDiagnostic>> takeDiagnostics();
};

#endif // VNLC_SEMANTIC_CONTEXT_HPP
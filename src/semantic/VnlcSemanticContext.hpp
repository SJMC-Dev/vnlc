#ifndef VNLC_SEMANTIC_CONTEXT_HPP
#define VNLC_SEMANTIC_CONTEXT_HPP

#include "../ast/type/VnlcTypeNode.hpp"
#include "../diagnostic/VnlcDiagnostic.hpp"
#include "scope/VnlcScope.hpp"
#include "type/VnlcReferenceType.hpp"
#include "type/VnlcSemanticType.hpp"
#include <memory>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

class VnlcSemanticContext {
private:
    std::vector<VnlcDiagnostic> errors;
    std::vector<VnlcDiagnostic> warnings;
    std::vector<VnlcDiagnostic> notes;

    std::vector<std::unique_ptr<VnlcScope>> scopeStack;

    std::unordered_map<std::string, std::unique_ptr<VnlcReferenceType>> referenceTypeRegistry;
    std::unordered_map<const VnlcTypeNode*, const VnlcSemanticType*> semanticTypeMap;

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

    void registerReferenceType(std::string_view fullName, std::unique_ptr<VnlcReferenceType>&& referenceType);
    void mapSemanticType(const VnlcTypeNode* typeNode, const VnlcSemanticType* semanticType);

    [[nodiscard]] const std::optional<const VnlcReferenceType*> getReferenceTypeByFullTypeName(std::string_view fullTypeName) const;
    [[nodiscard]] const std::optional<const VnlcSemanticType*> getSemanticTypeByTypeNode(const VnlcTypeNode* typeNode) const;

    [[nodiscard]] VnlcScope& currentScope();

    [[nodiscard]] const VnlcScope* currentModule();
    [[nodiscard]] const VnlcScope* currentFunction();
    [[nodiscard]] const VnlcScope* currentClass();
    [[nodiscard]] const VnlcScope* currentInterface();
    [[nodiscard]] const VnlcScope* currentEnum();
    [[nodiscard]] const VnlcScope* currentBlock();
    [[nodiscard]] const VnlcScope* currentLoop();
    [[nodiscard]] const VnlcScope* currentSwitch();

    [[nodiscard]] const std::vector<VnlcDiagnostic>& getErrors() const noexcept;
    [[nodiscard]] const std::vector<VnlcDiagnostic>& getWarnings() const noexcept;
    [[nodiscard]] const std::vector<VnlcDiagnostic>& getNotes() const noexcept;

    [[nodiscard]] unsigned int getLoopDepth() const noexcept;
    [[nodiscard]] unsigned int getSwitchDepth() const noexcept;

    [[nodiscard]] std::tuple<std::vector<VnlcDiagnostic>, std::vector<VnlcDiagnostic>, std::vector<VnlcDiagnostic>> takeDiagnostics();
};

#endif // VNLC_SEMANTIC_CONTEXT_HPP
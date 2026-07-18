#ifndef VNLC_FUNCTION_DECLARATION_NODE_HPP
#define VNLC_FUNCTION_DECLARATION_NODE_HPP

#include "../statement/VnlcBlockStatementNode.hpp"
#include "../type/VnlcTypeAnnotationNode.hpp"
#include "VnlcDeclarationNode.hpp"
#include "VnlcFunctionDeclarationType.hpp"
#include <memory>
#include <optional>
#include <utility>

class VnlcFunctionDeclarationNode : public VnlcDeclarationNode {
private:
    VnlcFunctionDeclarationNode() = delete;

    VnlcFunctionDeclarationType::Kind kind;
    VnlcFunctionDeclarationType::Context context;
    VnlcFunctionDeclarationType::AccessModifier accessModifier;
    VnlcFunctionDeclarationType::Binding binding;

    std::string name;
    std::string uniqueName;
    std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>> parameters;
    std::optional<std::unique_ptr<VnlcTypeAnnotationNode>> returnType;
    std::optional<std::unique_ptr<VnlcBlockStatementNode>> body;

    void generateUniqueName() noexcept;

public:
    VnlcFunctionDeclarationNode(
        VnlcFunctionDeclarationType::Kind kind,
        VnlcFunctionDeclarationType::Context context,
        VnlcFunctionDeclarationType::AccessModifier accessModifier,
        VnlcFunctionDeclarationType::Binding binding,
        std::unique_ptr<std::string>&& name,
        std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>>&& parameters,
        std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>&& returnType,
        std::optional<std::unique_ptr<VnlcBlockStatementNode>>&& body,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    VnlcFunctionDeclarationNode(
        VnlcFunctionDeclarationType::Kind kind,
        VnlcFunctionDeclarationType::Context context,
        VnlcFunctionDeclarationType::AccessModifier accessModifier,
        VnlcFunctionDeclarationType::Binding binding,
        std::unique_ptr<std::string>&& name,
        std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>>&& parameters,
        std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>&& returnType,
        std::optional<std::unique_ptr<VnlcBlockStatementNode>>&& body,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken,
        std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
    ) noexcept;

    [[nodiscard]] const VnlcFunctionDeclarationType::Kind getKind() const noexcept;
    [[nodiscard]] const VnlcFunctionDeclarationType::Context getContext() const noexcept;
    [[nodiscard]] const VnlcFunctionDeclarationType::AccessModifier getAccessModifier() const noexcept;
    [[nodiscard]] const VnlcFunctionDeclarationType::Binding getBinding() const noexcept;
    [[nodiscard]] std::string_view getName() const noexcept;
    [[nodiscard]] std::string_view getUniqueName() const noexcept;
    [[nodiscard]] const std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>>& getParameters() const noexcept;
    [[nodiscard]] const std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>& getReturnType() const noexcept;
    [[nodiscard]] const std::optional<std::unique_ptr<VnlcBlockStatementNode>>& getBody() const noexcept;
};

#endif // VNLC_FUNCTION_DECLARATION_NODE_HPP
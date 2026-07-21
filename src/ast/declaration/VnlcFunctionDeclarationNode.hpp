#ifndef VNLC_FUNCTION_DECLARATION_NODE_HPP
#define VNLC_FUNCTION_DECLARATION_NODE_HPP

#include "../statement/VnlcBlockStatementNode.hpp"
#include "../type/VnlcTypeAnnotationNode.hpp"
#include "VnlcDeclarationNode.hpp"
#include "VnlcFunctionDeclarationType.hpp"
#include "VnlcValueDeclarationNode.hpp"
#include <memory>
#include <optional>

class VnlcFunctionDeclarationNode : public VnlcDeclarationNode {
private:
    VnlcFunctionDeclarationNode() = delete;

    VnlcFunctionDeclarationType::Kind kind;
    VnlcFunctionDeclarationType::Context context;
    VnlcFunctionDeclarationType::AccessModifier accessModifier;
    VnlcFunctionDeclarationType::Binding binding;

    std::string name;
    std::string uniqueName;
    std::vector<std::unique_ptr<VnlcValueDeclarationNode>> parameters;
    std::optional<std::unique_ptr<VnlcTypeAnnotationNode>> returnType;
    std::optional<std::unique_ptr<VnlcBlockStatementNode>> body;

    void generateUniqueName();

public:
    VnlcFunctionDeclarationNode(
        VnlcFunctionDeclarationType::Kind kind,
        VnlcFunctionDeclarationType::Context context,
        VnlcFunctionDeclarationType::AccessModifier accessModifier,
        VnlcFunctionDeclarationType::Binding binding,
        std::string&& name,
        std::vector<std::unique_ptr<VnlcValueDeclarationNode>>&& parameters,
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
        std::string&& name,
        std::vector<std::unique_ptr<VnlcValueDeclarationNode>>&& parameters,
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
    [[nodiscard]] const std::vector<std::unique_ptr<VnlcValueDeclarationNode>>& getParameters() const noexcept;
    [[nodiscard]] const std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>& getReturnType() const noexcept;
    [[nodiscard]] const std::optional<std::unique_ptr<VnlcBlockStatementNode>>& getBody() const noexcept;
};

#endif // VNLC_FUNCTION_DECLARATION_NODE_HPP
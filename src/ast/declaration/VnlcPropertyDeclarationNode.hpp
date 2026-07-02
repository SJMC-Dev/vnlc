#ifndef VNLC_PROPERTY_DECLARATION_NODE_HPP
#define VNLC_PROPERTY_DECLARATION_NODE_HPP

#include "../expression/VnlcExpressionNode.hpp"
#include "../type/VnlcTypeAnnotationNode.hpp"
#include "VnlcDeclarationNode.hpp"
#include "VnlcPropertyDeclarationType.hpp"

class VnlcPropertyDeclarationNode : public VnlcDeclarationNode {
private:
    VnlcPropertyDeclarationNode() = delete;

    VnlcPropertyDeclarationType::AccessModifier accessModifier;
    VnlcPropertyDeclarationType::Binding binding;
    std::string name;
    std::unique_ptr<VnlcTypeAnnotationNode> typeAnnotation;
    std::optional<std::unique_ptr<VnlcExpressionNode>> initializer;

public:
    VnlcPropertyDeclarationNode(
        VnlcPropertyDeclarationType::AccessModifier accessModifier,
        VnlcPropertyDeclarationType::Binding binding,
        std::string&& name,
        std::unique_ptr<VnlcTypeAnnotationNode>&& typeAnnotation,
        std::optional<std::unique_ptr<VnlcExpressionNode>>&& initializer,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    VnlcPropertyDeclarationNode(
        VnlcPropertyDeclarationType::AccessModifier accessModifier,
        VnlcPropertyDeclarationType::Binding binding,
        std::string&& name,
        std::unique_ptr<VnlcTypeAnnotationNode>&& typeAnnotation,
        std::optional<std::unique_ptr<VnlcExpressionNode>>&& initializer,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken,
        std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
    ) noexcept;

    [[nodiscard]] const VnlcPropertyDeclarationType::AccessModifier getAccessModifier() const noexcept;
    [[nodiscard]] const VnlcPropertyDeclarationType::Binding getBinding() const noexcept;
    [[nodiscard]] std::string_view getName() const noexcept;
    [[nodiscard]] const VnlcTypeAnnotationNode& getTypeAnnotation() const noexcept;
    [[nodiscard]] const std::optional<std::unique_ptr<VnlcExpressionNode>>& getInitializer() const noexcept;
};

#endif // VNLC_PROPERTY_DECLARATION_NODE_HPP
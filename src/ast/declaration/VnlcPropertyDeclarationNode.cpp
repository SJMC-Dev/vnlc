#include "VnlcPropertyDeclarationNode.hpp"

VnlcPropertyDeclarationNode::VnlcPropertyDeclarationNode(
    VnlcPropertyDeclarationType::AccessModifier accessModifier,
    VnlcPropertyDeclarationType::Binding binding,
    std::unique_ptr<VnlcIdentifierNode>&& name,
    std::unique_ptr<VnlcTypeNode>&& type,
    std::optional<std::unique_ptr<VnlcExpressionNode>>&& initializer,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken),
      accessModifier(accessModifier),
      binding(binding),
      name(std::move(name)),
      type(std::move(type)),
      initializer(std::move(initializer)) {}

VnlcPropertyDeclarationNode::VnlcPropertyDeclarationNode(
    VnlcPropertyDeclarationType::AccessModifier accessModifier,
    VnlcPropertyDeclarationType::Binding binding,
    std::unique_ptr<VnlcIdentifierNode>&& name,
    std::unique_ptr<VnlcTypeNode>&& type,
    std::optional<std::unique_ptr<VnlcExpressionNode>>&& initializer,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken,
    std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken, std::move(metadataTerms)),
      accessModifier(accessModifier),
      binding(binding),
      name(std::move(name)),
      type(std::move(type)),
      initializer(std::move(initializer)) {}

const VnlcPropertyDeclarationType::AccessModifier VnlcPropertyDeclarationNode::getAccessModifier() const noexcept {
    return accessModifier;
}

const VnlcPropertyDeclarationType::Binding VnlcPropertyDeclarationNode::getBinding() const noexcept {
    return binding;
}

const VnlcIdentifierNode& VnlcPropertyDeclarationNode::getName() const noexcept {
    return *name;
}

const VnlcTypeNode& VnlcPropertyDeclarationNode::getType() const noexcept {
    return *type;
}

const std::optional<std::unique_ptr<VnlcExpressionNode>>& VnlcPropertyDeclarationNode::getInitializer() const noexcept {
    return initializer;
}
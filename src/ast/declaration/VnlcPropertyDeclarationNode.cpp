#include "VnlcPropertyDeclarationNode.hpp"

VnlcPropertyDeclarationNode::VnlcPropertyDeclarationNode(
    VnlcPropertyDeclarationType::AccessModifier accessModifier,
    VnlcPropertyDeclarationType::Binding binding,
    std::string&& name,
    std::unique_ptr<VnlcTypeAnnotationNode>&& typeAnnotation,
    std::optional<std::unique_ptr<VnlcExpressionNode>>&& initializer,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken),
      accessModifier(accessModifier),
      binding(binding),
      name(std::move(name)),
      typeAnnotation(std::move(typeAnnotation)),
      initializer(std::move(initializer)) {}

VnlcPropertyDeclarationNode::VnlcPropertyDeclarationNode(
    VnlcPropertyDeclarationType::AccessModifier accessModifier,
    VnlcPropertyDeclarationType::Binding binding,
    std::string&& name,
    std::unique_ptr<VnlcTypeAnnotationNode>&& typeAnnotation,
    std::optional<std::unique_ptr<VnlcExpressionNode>>&& initializer,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken,
    std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken, std::move(metadataTerms)),
      accessModifier(accessModifier),
      binding(binding),
      name(std::move(name)),
      typeAnnotation(std::move(typeAnnotation)),
      initializer(std::move(initializer)) {}

const VnlcPropertyDeclarationType::AccessModifier VnlcPropertyDeclarationNode::getAccessModifier() const noexcept {
    return accessModifier;
}

const VnlcPropertyDeclarationType::Binding VnlcPropertyDeclarationNode::getBinding() const noexcept {
    return binding;
}

std::string_view VnlcPropertyDeclarationNode::getName() const noexcept {
    return name;
}

const VnlcTypeAnnotationNode& VnlcPropertyDeclarationNode::getTypeAnnotation() const noexcept {
    return *typeAnnotation;
}

const std::optional<std::unique_ptr<VnlcExpressionNode>>& VnlcPropertyDeclarationNode::getInitializer() const noexcept {
    return initializer;
}
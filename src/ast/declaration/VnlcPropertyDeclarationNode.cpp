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
    bool includeMetadata,
    std::optional<bool> deprecated,
    std::optional<bool> experimental,
    std::optional<bool> nowarnings,
    std::optional<std::string>&& minGameVersion,
    std::optional<std::string>&& maxGameVersion
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken, includeMetadata, deprecated, experimental, nowarnings, std::move(minGameVersion), std::move(maxGameVersion)),
      accessModifier(accessModifier),
      binding(binding),
      name(std::move(name)),
      typeAnnotation(std::move(typeAnnotation)),
      initializer(std::move(initializer)) {}

VnlcPropertyDeclarationType::AccessModifier VnlcPropertyDeclarationNode::getAccessModifier() const noexcept {
    return accessModifier;
}

VnlcPropertyDeclarationType::Binding VnlcPropertyDeclarationNode::getBinding() const noexcept {
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
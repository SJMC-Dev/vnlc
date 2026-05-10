#include "VnlcVariableDeclarationNode.hpp"
#include "VnlcVariableDeclarationType.hpp"

VnlcVariableDeclarationNode::VnlcVariableDeclarationNode(
    VnlcVariableDeclarationType type,
    std::string&& name,
    std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>&& typeAnnotation,
    std::unique_ptr<VnlcExpressionNode>&& initializer,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken),
      type(type),
      name(std::move(name)),
      typeAnnotation(std::move(typeAnnotation)),
      initializer(std::move(initializer)) {}

VnlcVariableDeclarationNode::VnlcVariableDeclarationNode(
    VnlcVariableDeclarationType type,
    std::string&& name,
    std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>&& typeAnnotation,
    std::unique_ptr<VnlcExpressionNode>&& initializer,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken,
    std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken, std::move(metadataTerms)),
      type(type),
      name(std::move(name)),
      typeAnnotation(std::move(typeAnnotation)),
      initializer(std::move(initializer)) {}

const VnlcVariableDeclarationType VnlcVariableDeclarationNode::getType() const noexcept {
    return type;
}

std::string_view VnlcVariableDeclarationNode::getName() const noexcept {
    return name;
}

const std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>& VnlcVariableDeclarationNode::getTypeAnnotation() const noexcept {
    return typeAnnotation;
}

const VnlcExpressionNode& VnlcVariableDeclarationNode::getInitializer() const noexcept {
    return *initializer;
}
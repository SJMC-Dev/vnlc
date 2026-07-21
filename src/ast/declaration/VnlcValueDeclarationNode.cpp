#include "VnlcValueDeclarationNode.hpp"
#include "VnlcValueDeclarationType.hpp"

VnlcValueDeclarationNode::VnlcValueDeclarationNode(
    VnlcValueDeclarationType::Kind kind,
    VnlcValueDeclarationType::Context context,
    VnlcValueDeclarationType::AccessModifier accessModifier,
    std::string&& name,
    std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>&& typeAnnotation,
    std::optional<std::unique_ptr<VnlcExpressionNode>>&& initializer,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken),
      kind(kind),
      context(context),
      accessModifier(accessModifier),
      name(std::move(name)),
      typeAnnotation(std::move(typeAnnotation)),
      initializer(std::move(initializer)) {}

VnlcValueDeclarationNode::VnlcValueDeclarationNode(
    VnlcValueDeclarationType::Kind kind,
    VnlcValueDeclarationType::Context context,
    VnlcValueDeclarationType::AccessModifier accessModifier,
    std::string&& name,
    std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>&& typeAnnotation,
    std::optional<std::unique_ptr<VnlcExpressionNode>>&& initializer,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken,
    std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken, std::move(metadataTerms)),
      kind(kind),
      context(context),
      accessModifier(accessModifier),
      name(std::move(name)),
      typeAnnotation(std::move(typeAnnotation)),
      initializer(std::move(initializer)) {}

const VnlcValueDeclarationType::Kind VnlcValueDeclarationNode::getKind() const noexcept {
    return kind;
}

const VnlcValueDeclarationType::Context VnlcValueDeclarationNode::getContext() const noexcept {
    return context;
}

const VnlcValueDeclarationType::AccessModifier VnlcValueDeclarationNode::getAccessModifier() const noexcept {
    return accessModifier;
}

std::string_view VnlcValueDeclarationNode::getName() const noexcept {
    return name;
}

const std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>& VnlcValueDeclarationNode::getTypeAnnotation() const noexcept {
    return typeAnnotation;
}

const std::optional<std::unique_ptr<VnlcExpressionNode>>& VnlcValueDeclarationNode::getInitializer() const noexcept {
    return initializer;
}
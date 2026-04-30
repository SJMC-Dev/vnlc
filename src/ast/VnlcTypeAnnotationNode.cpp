#include "VnlcTypeAnnotationNode.hpp"

VnlcTypeAnnotationNode::VnlcTypeAnnotationNode(
    std::unique_ptr<VnlcQualifiedNameNode>&& typeName,
    bool readonly,
    bool optional,
    std::vector<std::unique_ptr<VnlcTypeAnnotationNode>>&& genericArguments,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcAstNode(firstToken, lastToken),
      typeName(std::move(typeName)),
      readonly(readonly),
      optional(optional),
      genericArguments(std::move(genericArguments)) {}

const VnlcQualifiedNameNode& VnlcTypeAnnotationNode::getTypeName() const noexcept {
    return *typeName;
}

bool VnlcTypeAnnotationNode::isReadonly() const noexcept {
    return readonly;
}

bool VnlcTypeAnnotationNode::isOptional() const noexcept {
    return optional;
}

const std::vector<std::unique_ptr<VnlcTypeAnnotationNode>>& VnlcTypeAnnotationNode::getGenericArguments() const noexcept {
    return genericArguments;
}
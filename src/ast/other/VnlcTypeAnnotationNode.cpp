#include "VnlcTypeAnnotationNode.hpp"

VnlcTypeAnnotationNode::VnlcTypeAnnotationNode(
    std::vector<std::string>&& typeNameParts,
    bool readonly,
    bool optional,
    std::vector<std::unique_ptr<VnlcTypeAnnotationNode>>&& genericArguments,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcAstNode(firstToken, lastToken),
      typeNameParts(std::move(typeNameParts)),
      readonly(readonly),
      optional(optional),
      genericArguments(std::move(genericArguments)) {}

const std::vector<std::string>& VnlcTypeAnnotationNode::getTypeNameParts() const noexcept {
    return typeNameParts;
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
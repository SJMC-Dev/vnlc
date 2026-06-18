#include "VnlcTypeAnnotationNode.hpp"

VnlcTypeAnnotationNode::VnlcTypeAnnotationNode(bool readonly, std::unique_ptr<VnlcTypeNode>&& typeNode, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcAstNode(firstToken, lastToken),
      typeNode(std::move(typeNode)),
      readonly(readonly) {}

const VnlcTypeNode& VnlcTypeAnnotationNode::getTypeNode() const noexcept {
    return *typeNode;
}

const bool VnlcTypeAnnotationNode::isReadonly() const noexcept {
    return readonly;
}
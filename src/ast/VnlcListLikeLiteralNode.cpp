#include "VnlcListLikeLiteralNode.hpp"

VnlcListLikeLiteralNode::VnlcListLikeLiteralNode(
    VnlcListLikeLiteralType type,
    std::vector<std::unique_ptr<VnlcExpressionNode>>&& elements,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcLiteralNode(firstToken, lastToken),
      type(type),
      elements(std::move(elements)) {}

const VnlcListLikeLiteralType VnlcListLikeLiteralNode::getType() const noexcept {
    return type;
}

const std::vector<std::unique_ptr<VnlcExpressionNode>>& VnlcListLikeLiteralNode::getElements() const noexcept {
    return elements;
}
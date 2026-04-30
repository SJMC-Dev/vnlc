#include "VnlcListLikeLiteralExpressionNode.hpp"

VnlcListLikeLiteralExpressionNode::VnlcListLikeLiteralExpressionNode(
    VnlcListLikeLiteralExpressionType type,
    std::vector<std::unique_ptr<VnlcExpressionNode>>&& elements,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcLiteralExpressionNode(firstToken, lastToken),
      type(type),
      elements(std::move(elements)) {}

const VnlcListLikeLiteralExpressionType VnlcListLikeLiteralExpressionNode::getType() const noexcept {
    return type;
}

const std::vector<std::unique_ptr<VnlcExpressionNode>>& VnlcListLikeLiteralExpressionNode::getElements() const noexcept {
    return elements;
}
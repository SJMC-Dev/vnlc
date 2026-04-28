#include "VnlcNotEqualExpressionNode.hpp"

VnlcNotEqualExpressionNode::VnlcNotEqualExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& left,
    std::unique_ptr<VnlcExpressionNode>&& right,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      left(std::move(left)),
      right(std::move(right)) {}

const VnlcExpressionNode& VnlcNotEqualExpressionNode::getLeft() const noexcept {
    return *left;
}

const VnlcExpressionNode& VnlcNotEqualExpressionNode::getRight() const noexcept {
    return *right;
}

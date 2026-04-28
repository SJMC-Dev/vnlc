#include "VnlcBitwiseOrExpressionNode.hpp"

VnlcBitwiseOrExpressionNode::VnlcBitwiseOrExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& left,
    std::unique_ptr<VnlcExpressionNode>&& right,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      left(std::move(left)),
      right(std::move(right)) {}

const VnlcExpressionNode& VnlcBitwiseOrExpressionNode::getLeft() const noexcept {
    return *left;
}

const VnlcExpressionNode& VnlcBitwiseOrExpressionNode::getRight() const noexcept {
    return *right;
}

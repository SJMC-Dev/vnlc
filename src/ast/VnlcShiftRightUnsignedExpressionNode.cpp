#include "VnlcShiftRightUnsignedExpressionNode.hpp"

VnlcShiftRightUnsignedExpressionNode::VnlcShiftRightUnsignedExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& left,
    std::unique_ptr<VnlcExpressionNode>&& right,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      left(std::move(left)),
      right(std::move(right)) {}

const VnlcExpressionNode& VnlcShiftRightUnsignedExpressionNode::getLeft() const noexcept {
    return *left;
}

const VnlcExpressionNode& VnlcShiftRightUnsignedExpressionNode::getRight() const noexcept {
    return *right;
}
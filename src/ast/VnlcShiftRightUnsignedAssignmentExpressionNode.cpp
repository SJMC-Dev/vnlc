#include "VnlcShiftRightUnsignedAssignmentExpressionNode.hpp"

VnlcShiftRightUnsignedAssignmentExpressionNode::VnlcShiftRightUnsignedAssignmentExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& left,
    std::unique_ptr<VnlcExpressionNode>&& right,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      left(std::move(left)),
      right(std::move(right)) {}

const VnlcExpressionNode& VnlcShiftRightUnsignedAssignmentExpressionNode::getLeft() const noexcept {
    return *left;
}

const VnlcExpressionNode& VnlcShiftRightUnsignedAssignmentExpressionNode::getRight() const noexcept {
    return *right;
}

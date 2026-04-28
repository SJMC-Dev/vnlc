#include "VnlcBitwiseAndAssignmentExpressionNode.hpp"

VnlcBitwiseAndAssignmentExpressionNode::VnlcBitwiseAndAssignmentExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& left,
    std::unique_ptr<VnlcExpressionNode>&& right,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      left(std::move(left)),
      right(std::move(right)) {}

const VnlcExpressionNode& VnlcBitwiseAndAssignmentExpressionNode::getLeft() const noexcept {
    return *left;
}

const VnlcExpressionNode& VnlcBitwiseAndAssignmentExpressionNode::getRight() const noexcept {
    return *right;
}

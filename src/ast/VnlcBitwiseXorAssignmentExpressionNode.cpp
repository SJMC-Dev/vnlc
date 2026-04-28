#include "VnlcBitwiseXorAssignmentExpressionNode.hpp"

VnlcBitwiseXorAssignmentExpressionNode::VnlcBitwiseXorAssignmentExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& left,
    std::unique_ptr<VnlcExpressionNode>&& right,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      left(std::move(left)),
      right(std::move(right)) {}

const VnlcExpressionNode& VnlcBitwiseXorAssignmentExpressionNode::getLeft() const noexcept {
    return *left;
}

const VnlcExpressionNode& VnlcBitwiseXorAssignmentExpressionNode::getRight() const noexcept {
    return *right;
}

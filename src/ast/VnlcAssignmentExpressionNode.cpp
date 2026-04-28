#include "VnlcAssignmentExpressionNode.hpp"

VnlcAssignmentExpressionNode::VnlcAssignmentExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& left,
    std::unique_ptr<VnlcExpressionNode>&& right,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      left(std::move(left)),
      right(std::move(right)) {}

const VnlcExpressionNode& VnlcAssignmentExpressionNode::getLeft() const noexcept {
    return *left;
}

const VnlcExpressionNode& VnlcAssignmentExpressionNode::getRight() const noexcept {
    return *right;
}

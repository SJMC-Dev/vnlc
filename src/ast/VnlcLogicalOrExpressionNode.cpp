#include "VnlcLogicalOrExpressionNode.hpp"

VnlcLogicalOrExpressionNode::VnlcLogicalOrExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& left,
    std::unique_ptr<VnlcExpressionNode>&& right,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      left(std::move(left)),
      right(std::move(right)) {}

const VnlcExpressionNode& VnlcLogicalOrExpressionNode::getLeft() const noexcept {
    return *left;
}

const VnlcExpressionNode& VnlcLogicalOrExpressionNode::getRight() const noexcept {
    return *right;
}

#include "VnlcGreaterThanOrEqualExpressionNode.hpp"

VnlcGreaterThanOrEqualExpressionNode::VnlcGreaterThanOrEqualExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& left,
    std::unique_ptr<VnlcExpressionNode>&& right,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      left(std::move(left)),
      right(std::move(right)) {}

const VnlcExpressionNode& VnlcGreaterThanOrEqualExpressionNode::getLeft() const noexcept {
    return *left;
}

const VnlcExpressionNode& VnlcGreaterThanOrEqualExpressionNode::getRight() const noexcept {
    return *right;
}

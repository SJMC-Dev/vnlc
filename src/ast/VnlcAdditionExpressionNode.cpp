#include "VnlcAdditionExpressionNode.hpp"

VnlcAdditionExpressionNode::VnlcAdditionExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& left,
    std::unique_ptr<VnlcExpressionNode>&& right,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      left(std::move(left)),
      right(std::move(right)) {}

const VnlcExpressionNode& VnlcAdditionExpressionNode::getLeft() const noexcept {
    return *left;
}

const VnlcExpressionNode& VnlcAdditionExpressionNode::getRight() const noexcept {
    return *right;
}
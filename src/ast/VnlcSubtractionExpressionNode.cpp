#include "VnlcSubtractionExpressionNode.hpp"

VnlcSubtractionExpressionNode::VnlcSubtractionExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& left,
    std::unique_ptr<VnlcExpressionNode>&& right,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      left(std::move(left)),
      right(std::move(right)) {}

const VnlcExpressionNode& VnlcSubtractionExpressionNode::getLeft() const noexcept {
    return *left;
}

const VnlcExpressionNode& VnlcSubtractionExpressionNode::getRight() const noexcept {
    return *right;
}
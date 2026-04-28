#include "VnlcBinaryExpressionNode.hpp"
#include <utility>

VnlcBinaryExpressionNode::VnlcBinaryExpressionNode(
    VnlcBinaryExpressionType type,
    std::unique_ptr<VnlcExpressionNode>&& left,
    std::unique_ptr<VnlcExpressionNode>&& right,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      type(type),
      left(std::move(left)),
      right(std::move(right)) {}

const VnlcBinaryExpressionType VnlcBinaryExpressionNode::getType() const noexcept {
    return type;
}

const VnlcExpressionNode& VnlcBinaryExpressionNode::getLeft() const noexcept {
    return *left;
}

const VnlcExpressionNode& VnlcBinaryExpressionNode::getRight() const noexcept {
    return *right;
}

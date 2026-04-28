#include "VnlcModuloExpressionNode.hpp"

VnlcModuloExpressionNode::VnlcModuloExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& left,
    std::unique_ptr<VnlcExpressionNode>&& right,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      left(std::move(left)),
      right(std::move(right)) {}

const VnlcExpressionNode& VnlcModuloExpressionNode::getLeft() const noexcept {
    return *left;
}

const VnlcExpressionNode& VnlcModuloExpressionNode::getRight() const noexcept {
    return *right;
}
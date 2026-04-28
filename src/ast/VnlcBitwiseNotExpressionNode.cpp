#include "VnlcBitwiseNotExpressionNode.hpp"

VnlcBitwiseNotExpressionNode::VnlcBitwiseNotExpressionNode(std::unique_ptr<VnlcExpressionNode>&& operand, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      operand(std::move(operand)) {}

const VnlcExpressionNode& VnlcBitwiseNotExpressionNode::getOperand() const noexcept {
    return *operand;
}
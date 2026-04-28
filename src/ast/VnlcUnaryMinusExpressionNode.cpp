#include "VnlcUnaryMinusExpressionNode.hpp"

VnlcUnaryMinusExpressionNode::VnlcUnaryMinusExpressionNode(std::unique_ptr<VnlcExpressionNode>&& operand, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      operand(std::move(operand)) {}

const VnlcExpressionNode& VnlcUnaryMinusExpressionNode::getOperand() const noexcept {
    return *operand;
}
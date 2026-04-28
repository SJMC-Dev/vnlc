#include "VnlcUnaryPlusExpressionNode.hpp"

VnlcUnaryPlusExpressionNode::VnlcUnaryPlusExpressionNode(std::unique_ptr<VnlcExpressionNode>&& operand, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      operand(std::move(operand)) {}

const VnlcExpressionNode& VnlcUnaryPlusExpressionNode::getOperand() const noexcept {
    return *operand;
}
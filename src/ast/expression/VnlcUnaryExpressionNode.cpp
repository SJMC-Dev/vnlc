#include "VnlcUnaryExpressionNode.hpp"
#include <utility>

VnlcUnaryExpressionNode::VnlcUnaryExpressionNode(VnlcUnaryExpressionType type, std::unique_ptr<VnlcExpressionNode>&& operand, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      type(type),
      operand(std::move(operand)) {}

const VnlcUnaryExpressionType VnlcUnaryExpressionNode::getType() const noexcept {
    return type;
}

const VnlcExpressionNode& VnlcUnaryExpressionNode::getOperand() const noexcept {
    return *operand;
}

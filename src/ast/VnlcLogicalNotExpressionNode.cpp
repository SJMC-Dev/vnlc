#include "VnlcLogicalNotExpressionNode.hpp"

VnlcLogicalNotExpressionNode::VnlcLogicalNotExpressionNode(std::unique_ptr<VnlcExpressionNode>&& operand, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      operand(std::move(operand)) {}

const VnlcExpressionNode& VnlcLogicalNotExpressionNode::getOperand() const noexcept {
    return *operand;
}

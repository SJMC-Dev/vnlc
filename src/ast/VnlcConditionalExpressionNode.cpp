#include "VnlcConditionalExpressionNode.hpp"

VnlcConditionalExpressionNode::VnlcConditionalExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& condition,
    std::unique_ptr<VnlcExpressionNode>&& thenExpression,
    std::unique_ptr<VnlcExpressionNode>&& elseExpression,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      condition(std::move(condition)),
      thenExpression(std::move(thenExpression)),
      elseExpression(std::move(elseExpression)) {}

const VnlcExpressionNode& VnlcConditionalExpressionNode::getCondition() const noexcept {
    return *condition;
}

const VnlcExpressionNode& VnlcConditionalExpressionNode::getThenExpression() const noexcept {
    return *thenExpression;
}

const VnlcExpressionNode& VnlcConditionalExpressionNode::getElseExpression() const noexcept {
    return *elseExpression;
}

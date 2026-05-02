#include "VnlcExpressionStatementNode.hpp"

VnlcExpressionStatementNode::VnlcExpressionStatementNode(std::unique_ptr<VnlcExpressionNode>&& expression, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcStatementNode(firstToken, lastToken),
      expression(std::move(expression)) {}

const VnlcExpressionNode& VnlcExpressionStatementNode::getExpression() const noexcept {
    return *expression;
}
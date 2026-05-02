#include "VnlcWhileStatementNode.hpp"

VnlcWhileStatementNode::VnlcWhileStatementNode(
    std::unique_ptr<VnlcExpressionNode>&& condition,
    std::unique_ptr<VnlcStatementNode>&& body,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcLoopStatementNode(std::nullopt, firstToken, lastToken),
      condition(std::move(condition)),
      body(std::move(body)) {}

VnlcWhileStatementNode::VnlcWhileStatementNode(
    std::string&& label,
    std::unique_ptr<VnlcExpressionNode>&& condition,
    std::unique_ptr<VnlcStatementNode>&& body,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcLoopStatementNode(std::make_optional(std::move(label)), firstToken, lastToken),
      condition(std::move(condition)),
      body(std::move(body)) {}

const VnlcExpressionNode& VnlcWhileStatementNode::getCondition() const noexcept {
    return *condition;
}

const VnlcStatementNode& VnlcWhileStatementNode::getBody() const noexcept {
    return *body;
}
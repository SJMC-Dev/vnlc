#include "VnlcForStatementNode.hpp"

VnlcForStatementNode::VnlcForStatementNode(
    std::unique_ptr<VnlcValueDeclarationNode>&& loopVariable,
    std::unique_ptr<VnlcExpressionNode>&& iterableExpression,
    std::unique_ptr<VnlcStatementNode>&& body,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcLoopStatementNode(std::nullopt, firstToken, lastToken),
      loopVariable(std::move(loopVariable)),
      iterableExpression(std::move(iterableExpression)),
      body(std::move(body)) {}

VnlcForStatementNode::VnlcForStatementNode(
    std::string&& label,
    std::unique_ptr<VnlcValueDeclarationNode>&& loopVariable,
    std::unique_ptr<VnlcExpressionNode>&& iterableExpression,
    std::unique_ptr<VnlcStatementNode>&& body,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcLoopStatementNode(std::make_optional(std::move(label)), firstToken, lastToken),
      loopVariable(std::move(loopVariable)),
      iterableExpression(std::move(iterableExpression)),
      body(std::move(body)) {}

const VnlcValueDeclarationNode& VnlcForStatementNode::getLoopVariable() const noexcept {
    return *loopVariable;
}

const VnlcExpressionNode& VnlcForStatementNode::getIterableExpression() const noexcept {
    return *iterableExpression;
}

const VnlcStatementNode& VnlcForStatementNode::getBody() const noexcept {
    return *body;
}
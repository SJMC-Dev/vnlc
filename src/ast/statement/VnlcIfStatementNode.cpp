#include "VnlcIfStatementNode.hpp"

VnlcIfStatementNode::VnlcIfStatementNode(
    std::unique_ptr<VnlcExpressionNode>&& condition,
    std::unique_ptr<VnlcStatementNode>&& thenBranch,
    std::optional<std::unique_ptr<VnlcStatementNode>>&& elseBranch,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcControlFlowStatementNode(firstToken, lastToken),
      condition(std::move(condition)),
      thenBranch(std::move(thenBranch)),
      elseBranch(std::move(elseBranch)) {}

const VnlcExpressionNode& VnlcIfStatementNode::getCondition() const noexcept {
    return *condition;
}

const VnlcStatementNode& VnlcIfStatementNode::getThenBranch() const noexcept {
    return *thenBranch;
}

const std::optional<std::unique_ptr<VnlcStatementNode>>& VnlcIfStatementNode::getElseBranch() const noexcept {
    return elseBranch;
}
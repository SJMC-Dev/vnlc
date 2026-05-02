#include "VnlcReturnStatementNode.hpp"

VnlcReturnStatementNode::VnlcReturnStatementNode(const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcControlFlowStatementNode(firstToken, lastToken),
      returnValue(std::nullopt) {}

VnlcReturnStatementNode::VnlcReturnStatementNode(std::unique_ptr<VnlcExpressionNode>&& returnValue, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcControlFlowStatementNode(firstToken, lastToken),
      returnValue(std::make_optional(std::move(returnValue))) {}

const std::optional<std::unique_ptr<VnlcExpressionNode>>& VnlcReturnStatementNode::getReturnValue() const noexcept {
    return returnValue;
}
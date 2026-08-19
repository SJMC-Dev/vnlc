#include "VnlcBreakStatementNode.hpp"
#include <memory>

VnlcBreakStatementNode::VnlcBreakStatementNode(const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept : VnlcControlFlowStatementNode(firstToken, lastToken), label(std::nullopt) {}

VnlcBreakStatementNode::VnlcBreakStatementNode(std::unique_ptr<VnlcIdentifierNode>&& label, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcControlFlowStatementNode(firstToken, lastToken),
      label(std::make_optional(std::move(label))) {}

const std::optional<std::unique_ptr<VnlcIdentifierNode>>& VnlcBreakStatementNode::getLabel() const noexcept {
    return label;
}
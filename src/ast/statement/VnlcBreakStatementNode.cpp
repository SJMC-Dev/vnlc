#include "VnlcBreakStatementNode.hpp"

VnlcBreakStatementNode::VnlcBreakStatementNode(const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept : VnlcControlFlowStatementNode(firstToken, lastToken), label(std::nullopt) {}

VnlcBreakStatementNode::VnlcBreakStatementNode(std::string&& label, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcControlFlowStatementNode(firstToken, lastToken),
      label(std::make_optional(std::move(label))) {}

const std::optional<std::string>& VnlcBreakStatementNode::getLabel() const noexcept {
    return label;
}
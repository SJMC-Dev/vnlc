#include "VnlcContinueStatementNode.hpp"

VnlcContinueStatementNode::VnlcContinueStatementNode(const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcControlFlowStatementNode(firstToken, lastToken),
      label(std::nullopt) {}

VnlcContinueStatementNode::VnlcContinueStatementNode(std::string&& label, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcControlFlowStatementNode(firstToken, lastToken),
      label(std::make_optional(std::move(label))) {}

const std::optional<std::string>& VnlcContinueStatementNode::getLabel() const noexcept {
    return label;
}
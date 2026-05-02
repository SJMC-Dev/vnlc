#include "VnlcLoopStatementNode.hpp"

VnlcLoopStatementNode::VnlcLoopStatementNode(std::optional<std::string>&& label, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcControlFlowStatementNode(firstToken, lastToken),
      label(std::move(label)) {}

const std::optional<std::string>& VnlcLoopStatementNode::getLabel() const noexcept {
    return label;
}
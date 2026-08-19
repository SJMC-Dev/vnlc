#include "VnlcLoopStatementNode.hpp"

VnlcLoopStatementNode::VnlcLoopStatementNode(std::optional<std::unique_ptr<VnlcIdentifierNode>>&& label, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcControlFlowStatementNode(firstToken, lastToken),
      label(std::move(label)) {}

const std::optional<std::unique_ptr<VnlcIdentifierNode>>& VnlcLoopStatementNode::getLabel() const noexcept {
    return label;
}
#include "VnlcBlockStatementNode.hpp"

VnlcBlockStatementNode::VnlcBlockStatementNode(std::vector<std::unique_ptr<VnlcStatementNode>>&& statements, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcStatementNode(firstToken, lastToken),
      statements(std::move(statements)) {}

const std::vector<std::unique_ptr<VnlcStatementNode>>& VnlcBlockStatementNode::getStatements() const noexcept {
    return statements;
}
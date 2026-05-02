#include "VnlcVariableDeclarationStatementNode.hpp"

VnlcVariableDeclarationStatementNode::VnlcVariableDeclarationStatementNode(
    std::unique_ptr<VnlcVariableDeclarationNode>&& variableDeclaration,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcStatementNode(firstToken, lastToken),
      variableDeclaration(std::move(variableDeclaration)) {}

const VnlcVariableDeclarationNode& VnlcVariableDeclarationStatementNode::getVariableDeclaration() const noexcept {
    return *variableDeclaration;
}
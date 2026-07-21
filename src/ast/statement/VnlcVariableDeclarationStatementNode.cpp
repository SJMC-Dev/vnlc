#include "VnlcVariableDeclarationStatementNode.hpp"

VnlcVariableDeclarationStatementNode::VnlcVariableDeclarationStatementNode(
    std::unique_ptr<VnlcValueDeclarationNode>&& variableDeclaration,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcStatementNode(firstToken, lastToken),
      variableDeclaration(std::move(variableDeclaration)) {}

const VnlcValueDeclarationNode& VnlcVariableDeclarationStatementNode::getVariableDeclaration() const noexcept {
    return *variableDeclaration;
}
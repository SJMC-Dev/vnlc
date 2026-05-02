#pragma once

#ifndef VNLC_VARIABLE_DECLARATION_STATEMENT_NODE_HPP
#define VNLC_VARIABLE_DECLARATION_STATEMENT_NODE_HPP

#include "../declaration/VnlcVariableDeclarationNode.hpp"
#include "VnlcStatementNode.hpp"
#include <memory>

class VnlcVariableDeclarationStatementNode : public VnlcStatementNode {
private:
    VnlcVariableDeclarationStatementNode() = delete;

    std::unique_ptr<VnlcVariableDeclarationNode> variableDeclaration;

public:
    VnlcVariableDeclarationStatementNode(std::unique_ptr<VnlcVariableDeclarationNode>&& variableDeclaration, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    [[nodiscard]] const VnlcVariableDeclarationNode& getVariableDeclaration() const noexcept;
};

#endif // VNLC_VARIABLE_DECLARATION_STATEMENT_NODE_HPP
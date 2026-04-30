#pragma once

#ifndef VNLC_BLOCK_STATEMENT_NODE_HPP
#define VNLC_BLOCK_STATEMENT_NODE_HPP

#include "VnlcStatementNode.hpp"
#include <memory>
#include <vector>

class VnlcBlockStatementNode : public VnlcStatementNode {
private:
    VnlcBlockStatementNode() = delete;

    std::vector<std::unique_ptr<VnlcStatementNode>> statements;

public:
    VnlcBlockStatementNode(std::vector<std::unique_ptr<VnlcStatementNode>>&& statements, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const std::vector<std::unique_ptr<VnlcStatementNode>>& getStatements() const noexcept;
};

#endif // VNLC_BLOCK_STATEMENT_NODE_HPP
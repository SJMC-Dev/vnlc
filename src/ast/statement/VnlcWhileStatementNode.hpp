#pragma once

#ifndef VNLC_WHILE_STATEMENT_NODE_HPP
#define VNLC_WHILE_STATEMENT_NODE_HPP

#include "../expression/VnlcExpressionNode.hpp"
#include "VnlcLoopStatementNode.hpp"
#include <memory>

class VnlcWhileStatementNode : public VnlcLoopStatementNode {
private:
    VnlcWhileStatementNode() = delete;

    std::unique_ptr<VnlcExpressionNode> condition;
    std::unique_ptr<VnlcStatementNode> body;

public:
    VnlcWhileStatementNode(std::unique_ptr<VnlcExpressionNode>&& condition, std::unique_ptr<VnlcStatementNode>&& body, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    VnlcWhileStatementNode(
        std::string&& label,
        std::unique_ptr<VnlcExpressionNode>&& condition,
        std::unique_ptr<VnlcStatementNode>&& body,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    [[nodiscard]] const VnlcExpressionNode& getCondition() const noexcept;
    [[nodiscard]] const VnlcStatementNode& getBody() const noexcept;
};

#endif // VNLC_WHILE_STATEMENT_NODE_HPP
#pragma once

#ifndef VNLC_FOR_STATEMENT_NODE_HPP
#define VNLC_FOR_STATEMENT_NODE_HPP

#include "../declaration/VnlcVariableDeclarationType.hpp"
#include "../expression/VnlcExpressionNode.hpp"
#include "VnlcLoopStatementNode.hpp"
#include <memory>

class VnlcForStatementNode : public VnlcLoopStatementNode {
private:
    VnlcForStatementNode() = delete;

    VnlcVariableDeclarationType loopVariableKind;
    std::string loopVariableName;
    std::unique_ptr<VnlcExpressionNode> iterableExpression;
    std::unique_ptr<VnlcStatementNode> body;

public:
    VnlcForStatementNode(
        VnlcVariableDeclarationType loopVariableKind,
        std::string&& loopVariableName,
        std::unique_ptr<VnlcExpressionNode>&& iterableExpression,
        std::unique_ptr<VnlcStatementNode>&& body,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    VnlcForStatementNode(
        std::string&& label,
        VnlcVariableDeclarationType loopVariableKind,
        std::string&& loopVariableName,
        std::unique_ptr<VnlcExpressionNode>&& iterableExpression,
        std::unique_ptr<VnlcStatementNode>&& body,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    [[nodiscard]] const VnlcVariableDeclarationType getLoopVariableKind() const noexcept;
    [[nodiscard]] std::string_view getLoopVariableName() const noexcept;
    [[nodiscard]] const VnlcExpressionNode& getIterableExpression() const noexcept;
    [[nodiscard]] const VnlcStatementNode& getBody() const noexcept;
};

#endif // VNLC_FOR_STATEMENT_NODE_HPP
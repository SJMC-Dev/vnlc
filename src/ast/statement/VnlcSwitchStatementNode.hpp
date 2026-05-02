#pragma once

#ifndef VNLC_SWITCH_STATEMENT_NODE_HPP
#define VNLC_SWITCH_STATEMENT_NODE_HPP

#include "VnlcControlFlowStatementNode.hpp"
#include "VnlcSwitchStatementItem.hpp"
#include "VnlcSwitchStatementType.hpp"
#include <memory>
#include <vector>

class VnlcSwitchStatementNode : public VnlcControlFlowStatementNode {
private:
    VnlcSwitchStatementNode() = delete;

    std::unique_ptr<VnlcExpressionNode> switchExpression;
    VnlcSwitchStatementType switchType;
    std::vector<VnlcSwitchStatementItem::LiteralMatchItem> literalMatchItems; // empty if switchType is TYPE_MATCH
    std::vector<VnlcSwitchStatementItem::TypeMatchItem> typeMatchItems;       // empty if switchType is LITERAL_MATCH
    std::unique_ptr<VnlcStatementNode> defaultCaseBody;

public:
    VnlcSwitchStatementNode(
        std::unique_ptr<VnlcExpressionNode>&& switchExpression,
        std::vector<VnlcSwitchStatementItem::LiteralMatchItem>&& literalMatchItems,
        std::unique_ptr<VnlcStatementNode>&& defaultCaseBody,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    VnlcSwitchStatementNode(
        std::unique_ptr<VnlcExpressionNode>&& switchExpression,
        std::vector<VnlcSwitchStatementItem::TypeMatchItem>&& typeMatchItems,
        std::unique_ptr<VnlcStatementNode>&& defaultCaseBody,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    [[nodiscard]] const VnlcExpressionNode& getSwitchExpression() const noexcept;
    [[nodiscard]] const VnlcSwitchStatementType getSwitchType() const noexcept;
    [[nodiscard]] const std::vector<VnlcSwitchStatementItem::LiteralMatchItem>& getLiteralMatchItems() const noexcept;
    [[nodiscard]] const std::vector<VnlcSwitchStatementItem::TypeMatchItem>& getTypeMatchItems() const noexcept;
    [[nodiscard]] const VnlcStatementNode& getDefaultCaseBody() const noexcept;
};

#endif // VNLC_SWITCH_STATEMENT_NODE_HPP
#pragma once

#ifndef VNLC_CONDITIONAL_EXPRESSION_NODE_HPP
#define VNLC_CONDITIONAL_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcConditionalExpressionNode : public VnlcExpressionNode {
private:
    VnlcConditionalExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> condition;
    std::unique_ptr<VnlcExpressionNode> thenExpression;
    std::unique_ptr<VnlcExpressionNode> elseExpression;

public:
    VnlcConditionalExpressionNode(
        std::unique_ptr<VnlcExpressionNode>&& condition,
        std::unique_ptr<VnlcExpressionNode>&& thenExpression,
        std::unique_ptr<VnlcExpressionNode>&& elseExpression,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcExpressionNode& getCondition() const noexcept;
    const VnlcExpressionNode& getThenExpression() const noexcept;
    const VnlcExpressionNode& getElseExpression() const noexcept;
};

#endif // VNLC_CONDITIONAL_EXPRESSION_NODE_HPP

#pragma once

#ifndef VNLC_UNARY_MINUS_EXPRESSION_NODE_HPP
#define VNLC_UNARY_MINUS_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcUnaryMinusExpressionNode : public VnlcExpressionNode {
private:
    VnlcUnaryMinusExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> operand;

public:
    VnlcUnaryMinusExpressionNode(std::unique_ptr<VnlcExpressionNode>&& operand, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const VnlcExpressionNode& getOperand() const noexcept;
};

#endif // VNLC_UNARY_MINUS_EXPRESSION_NODE_HPP
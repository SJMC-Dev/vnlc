#pragma once

#ifndef VNLC_UNARY_PLUS_EXPRESSION_NODE_HPP
#define VNLC_UNARY_PLUS_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcUnaryPlusExpressionNode : public VnlcExpressionNode {
private:
    VnlcUnaryPlusExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> operand;

public:
    VnlcUnaryPlusExpressionNode(std::unique_ptr<VnlcExpressionNode>&& operand, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const VnlcExpressionNode& getOperand() const noexcept;
};

#endif // VNLC_UNARY_PLUS_EXPRESSION_NODE_HPP
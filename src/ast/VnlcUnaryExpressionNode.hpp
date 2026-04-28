#pragma once

#ifndef VNLC_UNARY_EXPRESSION_NODE_HPP
#define VNLC_UNARY_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include "VnlcUnaryExpressionType.hpp"
#include <memory>

class VnlcUnaryExpressionNode : public VnlcExpressionNode {
private:
    VnlcUnaryExpressionNode() = delete;

    VnlcUnaryExpressionType type;
    std::unique_ptr<VnlcExpressionNode> operand;

public:
    VnlcUnaryExpressionNode(VnlcUnaryExpressionType type, std::unique_ptr<VnlcExpressionNode>&& operand, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const VnlcUnaryExpressionType getType() const noexcept;
    const VnlcExpressionNode& getOperand() const noexcept;
};

#endif // VNLC_UNARY_EXPRESSION_NODE_HPP

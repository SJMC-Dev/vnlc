#pragma once

#ifndef VNLC_BITWISE_NOT_EXPRESSION_NODE_HPP
#define VNLC_BITWISE_NOT_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcBitwiseNotExpressionNode : public VnlcExpressionNode {
private:
    VnlcBitwiseNotExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> operand;

public:
    VnlcBitwiseNotExpressionNode(std::unique_ptr<VnlcExpressionNode>&& operand, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const VnlcExpressionNode& getOperand() const noexcept;
};

#endif // VNLC_BITWISE_NOT_EXPRESSION_NODE_HPP
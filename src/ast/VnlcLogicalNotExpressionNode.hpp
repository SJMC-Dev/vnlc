#pragma once

#ifndef VNLC_LOGICAL_NOT_EXPRESSION_NODE_HPP
#define VNLC_LOGICAL_NOT_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcLogicalNotExpressionNode : public VnlcExpressionNode {
private:
    VnlcLogicalNotExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> operand;

public:
    VnlcLogicalNotExpressionNode(std::unique_ptr<VnlcExpressionNode>&& operand, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const VnlcExpressionNode& getOperand() const noexcept;
};

#endif // VNLC_LOGICAL_NOT_EXPRESSION_NODE_HPP
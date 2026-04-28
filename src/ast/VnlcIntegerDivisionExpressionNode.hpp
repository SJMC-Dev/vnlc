#pragma once

#ifndef VNLC_INTEGER_DIVISION_EXPRESSION_NODE_HPP
#define VNLC_INTEGER_DIVISION_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcIntegerDivisionExpressionNode : public VnlcExpressionNode {
private:
    VnlcIntegerDivisionExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcIntegerDivisionExpressionNode(
        std::unique_ptr<VnlcExpressionNode>&& left,
        std::unique_ptr<VnlcExpressionNode>&& right,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_INTEGER_DIVISION_EXPRESSION_NODE_HPP
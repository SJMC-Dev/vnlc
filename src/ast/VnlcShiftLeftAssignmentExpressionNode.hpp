#pragma once

#ifndef VNLC_SHIFT_LEFT_ASSIGNMENT_EXPRESSION_NODE_HPP
#define VNLC_SHIFT_LEFT_ASSIGNMENT_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcShiftLeftAssignmentExpressionNode : public VnlcExpressionNode {
private:
    VnlcShiftLeftAssignmentExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcShiftLeftAssignmentExpressionNode(
        std::unique_ptr<VnlcExpressionNode>&& left,
        std::unique_ptr<VnlcExpressionNode>&& right,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_SHIFT_LEFT_ASSIGNMENT_EXPRESSION_NODE_HPP

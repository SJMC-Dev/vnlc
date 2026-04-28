#pragma once

#ifndef VNLC_SHIFT_RIGHT_ASSIGNMENT_EXPRESSION_NODE_HPP
#define VNLC_SHIFT_RIGHT_ASSIGNMENT_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcShiftRightAssignmentExpressionNode : public VnlcExpressionNode {
private:
    VnlcShiftRightAssignmentExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcShiftRightAssignmentExpressionNode(
        std::unique_ptr<VnlcExpressionNode>&& left,
        std::unique_ptr<VnlcExpressionNode>&& right,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_SHIFT_RIGHT_ASSIGNMENT_EXPRESSION_NODE_HPP

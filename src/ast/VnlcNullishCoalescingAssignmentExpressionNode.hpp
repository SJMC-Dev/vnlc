#pragma once

#ifndef VNLC_NULLISH_COALESCING_ASSIGNMENT_EXPRESSION_NODE_HPP
#define VNLC_NULLISH_COALESCING_ASSIGNMENT_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcNullishCoalescingAssignmentExpressionNode : public VnlcExpressionNode {
private:
    VnlcNullishCoalescingAssignmentExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcNullishCoalescingAssignmentExpressionNode(
        std::unique_ptr<VnlcExpressionNode>&& left,
        std::unique_ptr<VnlcExpressionNode>&& right,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_NULLISH_COALESCING_ASSIGNMENT_EXPRESSION_NODE_HPP

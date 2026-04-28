#pragma once

#ifndef VNLC_MULTIPLICATION_ASSIGNMENT_EXPRESSION_NODE_HPP
#define VNLC_MULTIPLICATION_ASSIGNMENT_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcMultiplicationAssignmentExpressionNode : public VnlcExpressionNode {
private:
    VnlcMultiplicationAssignmentExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcMultiplicationAssignmentExpressionNode(
        std::unique_ptr<VnlcExpressionNode>&& left,
        std::unique_ptr<VnlcExpressionNode>&& right,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_MULTIPLICATION_ASSIGNMENT_EXPRESSION_NODE_HPP

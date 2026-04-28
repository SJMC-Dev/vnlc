#pragma once

#ifndef VNLC_ADDITION_ASSIGNMENT_EXPRESSION_NODE_HPP
#define VNLC_ADDITION_ASSIGNMENT_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcAdditionAssignmentExpressionNode : public VnlcExpressionNode {
private:
    VnlcAdditionAssignmentExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcAdditionAssignmentExpressionNode(
        std::unique_ptr<VnlcExpressionNode>&& left,
        std::unique_ptr<VnlcExpressionNode>&& right,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_ADDITION_ASSIGNMENT_EXPRESSION_NODE_HPP

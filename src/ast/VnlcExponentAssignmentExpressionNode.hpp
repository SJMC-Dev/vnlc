#pragma once

#ifndef VNLC_EXPONENT_ASSIGNMENT_EXPRESSION_NODE_HPP
#define VNLC_EXPONENT_ASSIGNMENT_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcExponentAssignmentExpressionNode : public VnlcExpressionNode {
private:
    VnlcExponentAssignmentExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcExponentAssignmentExpressionNode(
        std::unique_ptr<VnlcExpressionNode>&& left,
        std::unique_ptr<VnlcExpressionNode>&& right,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_EXPONENT_ASSIGNMENT_EXPRESSION_NODE_HPP

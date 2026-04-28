#pragma once

#ifndef VNLC_ASSIGNMENT_EXPRESSION_NODE_HPP
#define VNLC_ASSIGNMENT_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcAssignmentExpressionNode : public VnlcExpressionNode {
private:
    VnlcAssignmentExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcAssignmentExpressionNode(std::unique_ptr<VnlcExpressionNode>&& left, std::unique_ptr<VnlcExpressionNode>&& right, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_ASSIGNMENT_EXPRESSION_NODE_HPP

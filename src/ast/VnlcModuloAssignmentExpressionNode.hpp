#pragma once

#ifndef VNLC_MODULO_ASSIGNMENT_EXPRESSION_NODE_HPP
#define VNLC_MODULO_ASSIGNMENT_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcModuloAssignmentExpressionNode : public VnlcExpressionNode {
private:
    VnlcModuloAssignmentExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcModuloAssignmentExpressionNode(
        std::unique_ptr<VnlcExpressionNode>&& left,
        std::unique_ptr<VnlcExpressionNode>&& right,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_MODULO_ASSIGNMENT_EXPRESSION_NODE_HPP

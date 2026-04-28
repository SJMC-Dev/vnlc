#pragma once

#ifndef VNLC_LOGICAL_OR_EXPRESSION_NODE_HPP
#define VNLC_LOGICAL_OR_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcLogicalOrExpressionNode : public VnlcExpressionNode {
private:
    VnlcLogicalOrExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcLogicalOrExpressionNode(std::unique_ptr<VnlcExpressionNode>&& left, std::unique_ptr<VnlcExpressionNode>&& right, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_LOGICAL_OR_EXPRESSION_NODE_HPP

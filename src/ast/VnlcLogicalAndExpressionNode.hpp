#pragma once

#ifndef VNLC_LOGICAL_AND_EXPRESSION_NODE_HPP
#define VNLC_LOGICAL_AND_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcLogicalAndExpressionNode : public VnlcExpressionNode {
private:
    VnlcLogicalAndExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcLogicalAndExpressionNode(std::unique_ptr<VnlcExpressionNode>&& left, std::unique_ptr<VnlcExpressionNode>&& right, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_LOGICAL_AND_EXPRESSION_NODE_HPP

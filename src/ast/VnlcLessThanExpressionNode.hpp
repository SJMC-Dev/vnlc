#pragma once

#ifndef VNLC_LESS_THAN_EXPRESSION_NODE_HPP
#define VNLC_LESS_THAN_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcLessThanExpressionNode : public VnlcExpressionNode {
private:
    VnlcLessThanExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcLessThanExpressionNode(std::unique_ptr<VnlcExpressionNode>&& left, std::unique_ptr<VnlcExpressionNode>&& right, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_LESS_THAN_EXPRESSION_NODE_HPP

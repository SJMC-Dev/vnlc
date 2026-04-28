#pragma once

#ifndef VNLC_GREATER_THAN_EXPRESSION_NODE_HPP
#define VNLC_GREATER_THAN_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcGreaterThanExpressionNode : public VnlcExpressionNode {
private:
    VnlcGreaterThanExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcGreaterThanExpressionNode(std::unique_ptr<VnlcExpressionNode>&& left, std::unique_ptr<VnlcExpressionNode>&& right, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_GREATER_THAN_EXPRESSION_NODE_HPP

#pragma once

#ifndef VNLC_BITWISE_AND_EXPRESSION_NODE_HPP
#define VNLC_BITWISE_AND_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcBitwiseAndExpressionNode : public VnlcExpressionNode {
private:
    VnlcBitwiseAndExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcBitwiseAndExpressionNode(std::unique_ptr<VnlcExpressionNode>&& left, std::unique_ptr<VnlcExpressionNode>&& right, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_BITWISE_AND_EXPRESSION_NODE_HPP

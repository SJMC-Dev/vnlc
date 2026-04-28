#pragma once

#ifndef VNLC_NOT_EQUAL_EXPRESSION_NODE_HPP
#define VNLC_NOT_EQUAL_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcNotEqualExpressionNode : public VnlcExpressionNode {
private:
    VnlcNotEqualExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcNotEqualExpressionNode(std::unique_ptr<VnlcExpressionNode>&& left, std::unique_ptr<VnlcExpressionNode>&& right, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_NOT_EQUAL_EXPRESSION_NODE_HPP

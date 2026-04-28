#pragma once

#ifndef VNLC_NULLISH_COALESCING_EXPRESSION_NODE_HPP
#define VNLC_NULLISH_COALESCING_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcNullishCoalescingExpressionNode : public VnlcExpressionNode {
private:
    VnlcNullishCoalescingExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcNullishCoalescingExpressionNode(
        std::unique_ptr<VnlcExpressionNode>&& left,
        std::unique_ptr<VnlcExpressionNode>&& right,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_NULLISH_COALESCING_EXPRESSION_NODE_HPP

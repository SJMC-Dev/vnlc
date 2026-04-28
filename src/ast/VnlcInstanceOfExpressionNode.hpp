#pragma once

#ifndef VNLC_INSTANCE_OF_EXPRESSION_NODE_HPP
#define VNLC_INSTANCE_OF_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcInstanceOfExpressionNode : public VnlcExpressionNode {
private:
    VnlcInstanceOfExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcInstanceOfExpressionNode(std::unique_ptr<VnlcExpressionNode>&& left, std::unique_ptr<VnlcExpressionNode>&& right, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_INSTANCE_OF_EXPRESSION_NODE_HPP

#pragma once

#ifndef VNLC_SUBTRACTION_EXPRESSION_NODE_HPP
#define VNLC_SUBTRACTION_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>

class VnlcSubtractionExpressionNode : public VnlcExpressionNode {
private:
    VnlcSubtractionExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> left;
    std::unique_ptr<VnlcExpressionNode> right;

public:
    VnlcSubtractionExpressionNode(std::unique_ptr<VnlcExpressionNode>&& left, std::unique_ptr<VnlcExpressionNode>&& right, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const VnlcExpressionNode& getLeft() const noexcept;
    const VnlcExpressionNode& getRight() const noexcept;
};

#endif // VNLC_SUBTRACTION_EXPRESSION_NODE_HPP
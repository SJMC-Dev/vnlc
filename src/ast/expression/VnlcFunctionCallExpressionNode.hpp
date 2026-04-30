#pragma once

#ifndef VNLC_FUNCTION_CALL_EXPRESSION_NODE_HPP
#define VNLC_FUNCTION_CALL_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include <memory>
#include <vector>

class VnlcFunctionCallExpressionNode : public VnlcExpressionNode {
private:
    VnlcFunctionCallExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> callee;
    std::vector<std::unique_ptr<VnlcExpressionNode>> arguments;

public:
    VnlcFunctionCallExpressionNode(
        std::unique_ptr<VnlcExpressionNode>&& callee,
        std::vector<std::unique_ptr<VnlcExpressionNode>>&& arguments,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcExpressionNode& getCallee() const noexcept;
    const std::vector<std::unique_ptr<VnlcExpressionNode>>& getArguments() const noexcept;
};

#endif // VNLC_FUNCTION_CALL_EXPRESSION_NODE_HPP
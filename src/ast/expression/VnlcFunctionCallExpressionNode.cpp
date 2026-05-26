#include "VnlcFunctionCallExpressionNode.hpp"

VnlcFunctionCallExpressionNode::VnlcFunctionCallExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& callee,
    std::vector<std::unique_ptr<VnlcExpressionNode>>&& arguments,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      callee(std::move(callee)),
      arguments(std::move(arguments)),
      context(std::nullopt) {}

VnlcFunctionCallExpressionNode::VnlcFunctionCallExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& callee,
    std::vector<std::unique_ptr<VnlcExpressionNode>>&& arguments,
    std::unique_ptr<VnlcExpressionNode>&& context,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      callee(std::move(callee)),
      arguments(std::move(arguments)),
      context(std::move(context)) {}

const VnlcExpressionNode& VnlcFunctionCallExpressionNode::getCallee() const noexcept {
    return *callee;
}

const std::vector<std::unique_ptr<VnlcExpressionNode>>& VnlcFunctionCallExpressionNode::getArguments() const noexcept {
    return arguments;
}

const std::optional<std::unique_ptr<VnlcExpressionNode>>& VnlcFunctionCallExpressionNode::getContext() const noexcept {
    return context;
}
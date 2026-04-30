#include "VnlcSelectorLiteralExpressionNode.hpp"

VnlcSelectorLiteralExpressionNode::VnlcSelectorLiteralExpressionNode(
    VnlcSelectorLiteralExpressionType type,
    std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>>&& arguments,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcLiteralExpressionNode(firstToken, lastToken),
      type(type),
      arguments(std::move(arguments)) {}

const VnlcSelectorLiteralExpressionType VnlcSelectorLiteralExpressionNode::getType() const noexcept {
    return type;
}

const std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>>& VnlcSelectorLiteralExpressionNode::getArguments() const noexcept {
    return arguments;
}
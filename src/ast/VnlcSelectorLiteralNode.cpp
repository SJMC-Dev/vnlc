#include "VnlcSelectorLiteralNode.hpp"

VnlcSelectorLiteralNode::VnlcSelectorLiteralNode(
    VnlcSelectorLiteralType type,
    std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>>&& arguments,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcLiteralNode(firstToken, lastToken),
      type(type),
      arguments(std::move(arguments)) {}

const VnlcSelectorLiteralType VnlcSelectorLiteralNode::getType() const noexcept {
    return type;
}

const std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>>& VnlcSelectorLiteralNode::getArguments() const noexcept {
    return arguments;
}
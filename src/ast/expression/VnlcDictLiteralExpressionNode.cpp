#include "VnlcDictLiteralExpressionNode.hpp"

VnlcDictLiteralExpressionNode::VnlcDictLiteralExpressionNode(
    std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>>&& entries,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcLiteralExpressionNode(firstToken, lastToken),
      entries(std::move(entries)) {}

const std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>>& VnlcDictLiteralExpressionNode::getEntries() const noexcept {
    return entries;
}
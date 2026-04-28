#include "VnlcDictLiteralNode.hpp"

VnlcDictLiteralNode::VnlcDictLiteralNode(std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>>&& entries, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcLiteralNode(firstToken, lastToken),
      entries(std::move(entries)) {}

const std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>>& VnlcDictLiteralNode::getEntries() const noexcept {
    return entries;
}
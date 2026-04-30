#include "VnlcStringLiteralExpressionNode.hpp"

VnlcStringLiteralExpressionNode::VnlcStringLiteralExpressionNode(
    VnlcStringLiteralExpressionType type,
    std::vector<std::variant<std::string, std::unique_ptr<VnlcExpressionNode>>>&& parts,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcLiteralExpressionNode(firstToken, lastToken),
      type(type),
      parts(std::move(parts)) {}

const VnlcStringLiteralExpressionType VnlcStringLiteralExpressionNode::getType() const noexcept {
    return type;
}

const std::vector<std::variant<std::string, std::unique_ptr<VnlcExpressionNode>>>& VnlcStringLiteralExpressionNode::getParts() const noexcept {
    return parts;
}
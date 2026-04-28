#include "VnlcStringLiteralNode.hpp"

VnlcStringLiteralNode::VnlcStringLiteralNode(
    VnlcStringLiteralType type,
    std::vector<std::variant<std::string, std::unique_ptr<VnlcExpressionNode>>>&& parts,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcLiteralNode(firstToken, lastToken),
      type(type),
      parts(std::move(parts)) {}

const VnlcStringLiteralType VnlcStringLiteralNode::getType() const noexcept {
    return type;
}

const std::vector<std::variant<std::string, std::unique_ptr<VnlcExpressionNode>>>& VnlcStringLiteralNode::getParts() const noexcept {
    return parts;
}
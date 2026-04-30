#include "VnlcSimpleLiteralExpressionNode.hpp"

VnlcSimpleLiteralExpressionNode::VnlcSimpleLiteralExpressionNode(VnlcSimpleLiteralExpressionType type, std::string&& literal, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcLiteralExpressionNode(firstToken, lastToken),
      type(type),
      literal(std::move(literal)) {}

const VnlcSimpleLiteralExpressionType VnlcSimpleLiteralExpressionNode::getType() const noexcept {
    return type;
}

std::string_view VnlcSimpleLiteralExpressionNode::getLiteral() const noexcept {
    return literal;
}
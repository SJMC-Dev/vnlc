#include "VnlcSimpleLiteralNode.hpp"

VnlcSimpleLiteralNode::VnlcSimpleLiteralNode(VnlcSimpleLiteralType type, std::string&& literal, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcLiteralNode(firstToken, lastToken),
      type(type),
      literal(std::move(literal)) {}

const VnlcSimpleLiteralType VnlcSimpleLiteralNode::getType() const noexcept {
    return type;
}

std::string_view VnlcSimpleLiteralNode::getLiteral() const noexcept {
    return literal;
}
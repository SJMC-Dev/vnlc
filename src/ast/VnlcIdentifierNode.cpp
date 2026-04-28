#include "VnlcIdentifierNode.hpp"

VnlcIdentifierNode::VnlcIdentifierNode(std::string&& name, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcPrimaryExpressionNode(firstToken, lastToken),
      name(std::move(name)) {}

std::string_view VnlcIdentifierNode::getName() const noexcept {
    return name;
}

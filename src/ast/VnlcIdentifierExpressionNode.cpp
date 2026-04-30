#include "VnlcIdentifierExpressionNode.hpp"

VnlcIdentifierExpressionNode::VnlcIdentifierExpressionNode(std::string&& name, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcPrimaryExpressionNode(firstToken, lastToken),
      name(std::move(name)) {}

std::string_view VnlcIdentifierExpressionNode::getName() const noexcept {
    return name;
}

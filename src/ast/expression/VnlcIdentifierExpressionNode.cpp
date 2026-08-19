#include "VnlcIdentifierExpressionNode.hpp"

VnlcIdentifierExpressionNode::VnlcIdentifierExpressionNode(std::unique_ptr<VnlcIdentifierNode>&& name, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcPrimaryExpressionNode(firstToken, lastToken),
      name(std::move(name)) {}

const VnlcIdentifierNode& VnlcIdentifierExpressionNode::getName() const noexcept {
    return *name;
}

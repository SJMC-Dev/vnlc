#include "VnlcSubscriptExpressionNode.hpp"

VnlcSubscriptExpressionNode::VnlcSubscriptExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& object,
    std::unique_ptr<VnlcExpressionNode>&& index,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      object(std::move(object)),
      index(std::move(index)) {}

const VnlcExpressionNode& VnlcSubscriptExpressionNode::getObject() const noexcept {
    return *object;
}

const VnlcExpressionNode& VnlcSubscriptExpressionNode::getIndex() const noexcept {
    return *index;
}
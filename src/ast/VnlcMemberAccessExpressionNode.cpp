#include "VnlcMemberAccessExpressionNode.hpp"

VnlcMemberAccessExpressionNode::VnlcMemberAccessExpressionNode(
    VnlcMemberAccessExpressionType type,
    std::unique_ptr<VnlcExpressionNode>&& object,
    std::unique_ptr<VnlcIdentifierNode>&& member,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      type(type),
      object(std::move(object)),
      member(std::move(member)) {}

const VnlcMemberAccessExpressionType VnlcMemberAccessExpressionNode::getType() const noexcept {
    return type;
}

const VnlcExpressionNode& VnlcMemberAccessExpressionNode::getObject() const noexcept {
    return *object;
}

const VnlcIdentifierNode& VnlcMemberAccessExpressionNode::getMember() const noexcept {
    return *member;
}
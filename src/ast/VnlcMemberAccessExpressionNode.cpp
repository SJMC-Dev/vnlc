#include "VnlcMemberAccessExpressionNode.hpp"

VnlcMemberAccessExpressionNode::VnlcMemberAccessExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& object,
    std::unique_ptr<VnlcIdentifierNode>&& member,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      object(std::move(object)),
      member(std::move(member)) {}

const VnlcExpressionNode& VnlcMemberAccessExpressionNode::getObject() const noexcept {
    return *object;
}

const VnlcIdentifierNode& VnlcMemberAccessExpressionNode::getMember() const noexcept {
    return *member;
}
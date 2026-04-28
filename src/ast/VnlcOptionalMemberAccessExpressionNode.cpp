#include "VnlcOptionalMemberAccessExpressionNode.hpp"

VnlcOptionalMemberAccessExpressionNode::VnlcOptionalMemberAccessExpressionNode(
    std::unique_ptr<VnlcExpressionNode>&& object,
    std::unique_ptr<VnlcIdentifierNode>&& member,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      object(std::move(object)),
      member(std::move(member)) {}

const VnlcExpressionNode& VnlcOptionalMemberAccessExpressionNode::getObject() const noexcept {
    return *object;
}

const VnlcIdentifierNode& VnlcOptionalMemberAccessExpressionNode::getMember() const noexcept {
    return *member;
}
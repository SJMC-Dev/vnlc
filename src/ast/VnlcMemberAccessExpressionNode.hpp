#pragma once

#ifndef VNLC_MEMBER_ACCESS_EXPRESSION_NODE_HPP
#define VNLC_MEMBER_ACCESS_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include "VnlcIdentifierNode.hpp"
#include "VnlcMemberAccessExpressionType.hpp"
#include <memory>

class VnlcMemberAccessExpressionNode : public VnlcExpressionNode {
private:
    VnlcMemberAccessExpressionNode() = delete;

    VnlcMemberAccessExpressionType type;
    std::unique_ptr<VnlcExpressionNode> object;
    std::unique_ptr<VnlcIdentifierNode> member;

public:
    VnlcMemberAccessExpressionNode(
        VnlcMemberAccessExpressionType type,
        std::unique_ptr<VnlcExpressionNode>&& object,
        std::unique_ptr<VnlcIdentifierNode>&& member,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcMemberAccessExpressionType getType() const noexcept;
    const VnlcExpressionNode& getObject() const noexcept;
    const VnlcIdentifierNode& getMember() const noexcept;
};

#endif // VNLC_MEMBER_ACCESS_EXPRESSION_NODE_HPP
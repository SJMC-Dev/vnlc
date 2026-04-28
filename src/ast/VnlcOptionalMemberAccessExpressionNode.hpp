#pragma once

#ifndef VNLC_OPTIONAL_MEMBER_ACCESS_EXPRESSION_NODE_HPP
#define VNLC_OPTIONAL_MEMBER_ACCESS_EXPRESSION_NODE_HPP

#include "VnlcExpressionNode.hpp"
#include "VnlcIdentifierNode.hpp"
#include <memory>

class VnlcOptionalMemberAccessExpressionNode : public VnlcExpressionNode {
private:
    VnlcOptionalMemberAccessExpressionNode() = delete;

    std::unique_ptr<VnlcExpressionNode> object;
    std::unique_ptr<VnlcIdentifierNode> member;

public:
    VnlcOptionalMemberAccessExpressionNode(
        std::unique_ptr<VnlcExpressionNode>&& object,
        std::unique_ptr<VnlcIdentifierNode>&& member,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcExpressionNode& getObject() const noexcept;
    const VnlcIdentifierNode& getMember() const noexcept;
};

#endif // VNLC_OPTIONAL_MEMBER_ACCESS_EXPRESSION_NODE_HPP
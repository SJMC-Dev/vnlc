#pragma once

#ifndef VNLC_LIST_LIKE_LITERAL_NODE_HPP
#define VNLC_LIST_LIKE_LITERAL_NODE_HPP

#include "VnlcListLikeLiteralType.hpp"
#include "VnlcLiteralNode.hpp"
#include <memory>
#include <vector>

class VnlcListLikeLiteralNode : public VnlcLiteralNode {
private:
    VnlcListLikeLiteralNode() = delete;

    VnlcListLikeLiteralType type;
    std::vector<std::unique_ptr<VnlcExpressionNode>> elements;

public:
    VnlcListLikeLiteralNode(VnlcListLikeLiteralType type, std::vector<std::unique_ptr<VnlcExpressionNode>>&& elements, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const VnlcListLikeLiteralType getType() const noexcept;
    const std::vector<std::unique_ptr<VnlcExpressionNode>>& getElements() const noexcept;
};

#endif // VNLC_LIST_LIKE_LITERAL_NODE_HPP
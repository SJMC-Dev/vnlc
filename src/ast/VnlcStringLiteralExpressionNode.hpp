#pragma once

#ifndef VNLC_STRING_LITERAL_EXPRESSION_NODE_HPP
#define VNLC_STRING_LITERAL_EXPRESSION_NODE_HPP

#include "VnlcLiteralExpressionNode.hpp"
#include "VnlcStringLiteralExpressionType.hpp"
#include <memory>
#include <variant>
#include <vector>

class VnlcStringLiteralExpressionNode : public VnlcLiteralExpressionNode {
private:
    VnlcStringLiteralExpressionNode() = delete;

    VnlcStringLiteralExpressionType type;
    std::vector<std::variant<std::string, std::unique_ptr<VnlcExpressionNode>>> parts;

public:
    VnlcStringLiteralExpressionNode(
        VnlcStringLiteralExpressionType type,
        std::vector<std::variant<std::string, std::unique_ptr<VnlcExpressionNode>>>&& parts,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcStringLiteralExpressionType getType() const noexcept;
    const std::vector<std::variant<std::string, std::unique_ptr<VnlcExpressionNode>>>& getParts() const noexcept;
};

#endif // VNLC_STRING_LITERAL_EXPRESSION_NODE_HPP
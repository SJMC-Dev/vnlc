#pragma once

#ifndef VNLC_STRING_LITERAL_NODE_HPP
#define VNLC_STRING_LITERAL_NODE_HPP

#include "VnlcLiteralNode.hpp"
#include "VnlcStringLiteralType.hpp"
#include <memory>
#include <variant>
#include <vector>

class VnlcStringLiteralNode : public VnlcLiteralNode {
private:
    VnlcStringLiteralNode() = delete;

    VnlcStringLiteralType type;
    std::vector<std::variant<std::string, std::unique_ptr<VnlcExpressionNode>>> parts;

public:
    VnlcStringLiteralNode(
        VnlcStringLiteralType type,
        std::vector<std::variant<std::string, std::unique_ptr<VnlcExpressionNode>>>&& parts,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcStringLiteralType getType() const noexcept;
    const std::vector<std::variant<std::string, std::unique_ptr<VnlcExpressionNode>>>& getParts() const noexcept;
};

#endif // VNLC_STRING_LITERAL_NODE_HPP
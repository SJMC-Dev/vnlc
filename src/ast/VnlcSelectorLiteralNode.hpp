#pragma once

#ifndef VNLC_SELECTOR_LITERAL_NODE_HPP
#define VNLC_SELECTOR_LITERAL_NODE_HPP

#include "VnlcLiteralNode.hpp"
#include "VnlcSelectorLiteralType.hpp"
#include <memory>
#include <unordered_map>

class VnlcSelectorLiteralNode : public VnlcLiteralNode {
private:
    VnlcSelectorLiteralNode() = delete;

    VnlcSelectorLiteralType type;
    std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>> arguments;

public:
    VnlcSelectorLiteralNode(
        VnlcSelectorLiteralType type,
        std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>>&& arguments,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcSelectorLiteralType getType() const noexcept;
    const std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>>& getArguments() const noexcept;
};

#endif // VNLC_SELECTOR_LITERAL_NODE_HPP
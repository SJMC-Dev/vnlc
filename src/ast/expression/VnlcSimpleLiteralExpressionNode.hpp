#pragma once

#ifndef VNLC_SIMPLE_LITERAL_EXPRESSION_NODE_HPP
#define VNLC_SIMPLE_LITERAL_EXPRESSION_NODE_HPP

#include "VnlcLiteralExpressionNode.hpp"
#include "VnlcSimpleLiteralExpressionType.hpp"

class VnlcSimpleLiteralExpressionNode : public VnlcLiteralExpressionNode {
private:
    VnlcSimpleLiteralExpressionNode() = delete;

    VnlcSimpleLiteralExpressionType type;
    std::string literal;

public:
    VnlcSimpleLiteralExpressionNode(VnlcSimpleLiteralExpressionType type, std::string&& literal, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const VnlcSimpleLiteralExpressionType getType() const noexcept;
    std::string_view getLiteral() const noexcept;
};

#endif // VNLC_SIMPLE_LITERAL_EXPRESSION_NODE_HPP
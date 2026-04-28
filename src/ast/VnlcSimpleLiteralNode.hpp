#pragma once

#ifndef VNLC_SIMPLE_LITERAL_NODE_HPP
#define VNLC_SIMPLE_LITERAL_NODE_HPP

#include "VnlcLiteralNode.hpp"
#include "VnlcSimpleLiteralType.hpp"

class VnlcSimpleLiteralNode : public VnlcLiteralNode {
private:
    VnlcSimpleLiteralNode() = delete;

    VnlcSimpleLiteralType type;
    std::string literal;

public:
    VnlcSimpleLiteralNode(VnlcSimpleLiteralType type, std::string&& literal, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const VnlcSimpleLiteralType getType() const noexcept;
    std::string_view getLiteral() const noexcept;
};

#endif // VNLC_SIMPLE_LITERAL_NODE_HPP
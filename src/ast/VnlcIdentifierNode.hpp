#pragma once

#ifndef VNLC_IDENTIFIER_NODE_HPP
#define VNLC_IDENTIFIER_NODE_HPP

#include "VnlcExpressionNode.hpp"

class VnlcIdentifierNode : public VnlcExpressionNode {
private:
    VnlcIdentifierNode() = delete;

    std::string name;

public:
    VnlcIdentifierNode(std::string&& name, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    std::string_view getName() const noexcept;
};

#endif // VNLC_IDENTIFIER_NODE_HPP
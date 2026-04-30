#pragma once

#ifndef VNLC_IDENTIFIER_EXPRESSION_NODE_HPP
#define VNLC_IDENTIFIER_EXPRESSION_NODE_HPP

#include "VnlcPrimaryExpressionNode.hpp"

class VnlcIdentifierExpressionNode : public VnlcPrimaryExpressionNode {
private:
    VnlcIdentifierExpressionNode() = delete;

    std::string name;

public:
    VnlcIdentifierExpressionNode(std::string&& name, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    std::string_view getName() const noexcept;
};

#endif // VNLC_IDENTIFIER_EXPRESSION_NODE_HPP

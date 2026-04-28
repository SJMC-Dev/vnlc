#pragma once

#ifndef VNLC_LITERAL_NODE_HPP
#define VNLC_LITERAL_NODE_HPP

#include "VnlcPrimaryExpressionNode.hpp"

class VnlcLiteralNode : public VnlcPrimaryExpressionNode {
private:
    VnlcLiteralNode() = delete;

protected:
    VnlcLiteralNode(const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;
};

#endif // VNLC_LITERAL_NODE_HPP

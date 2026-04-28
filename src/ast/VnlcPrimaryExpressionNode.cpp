#include "VnlcPrimaryExpressionNode.hpp"

VnlcPrimaryExpressionNode::VnlcPrimaryExpressionNode(const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcExpressionNode(firstToken, lastToken) {}

#include "VnlcIdentifierNode.hpp"
#include <string_view>

VnlcIdentifierNode::VnlcIdentifierNode(std::string_view identifierString, const VnlcToken& firstToken, const VnlcToken& lastToken)
    : VnlcAstNode(firstToken, lastToken),
      identifierString(identifierString) {}

std::string_view VnlcIdentifierNode::getIdentifierString() const {
    return identifierString;
}
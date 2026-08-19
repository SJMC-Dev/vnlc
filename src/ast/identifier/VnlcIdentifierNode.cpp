#include "VnlcIdentifierNode.hpp"
#include <string_view>

VnlcIdentifierNode::VnlcIdentifierNode(std::string_view name, const VnlcToken& firstToken, const VnlcToken& lastToken) : VnlcAstNode(firstToken, lastToken), name(name) {}

std::string_view VnlcIdentifierNode::getName() const {
    return name;
}
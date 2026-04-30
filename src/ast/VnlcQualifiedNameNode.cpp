#include "VnlcQualifiedNameNode.hpp"

VnlcQualifiedNameNode::VnlcQualifiedNameNode(std::vector<std::string>&& parts, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcAstNode(firstToken, lastToken),
      parts(std::move(parts)) {}

const std::vector<std::string>& VnlcQualifiedNameNode::getParts() const noexcept {
    return parts;
}
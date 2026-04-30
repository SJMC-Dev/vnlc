#pragma once

#ifndef VNLC_QUALIFIED_NAME_NODE_HPP
#define VNLC_QUALIFIED_NAME_NODE_HPP

#include "VnlcAstNode.hpp"
#include <vector>

class VnlcQualifiedNameNode : public VnlcAstNode {
private:
    VnlcQualifiedNameNode() = delete;

    std::vector<std::string> parts;

public:
    VnlcQualifiedNameNode(std::vector<std::string>&& parts, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const std::vector<std::string>& getParts() const noexcept;
};

#endif // VNLC_QUALIFIED_NAME_NODE_HPP
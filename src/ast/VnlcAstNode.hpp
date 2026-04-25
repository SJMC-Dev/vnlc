#pragma once

#ifndef VNLC_AST_NODE_HPP
#define VNLC_AST_NODE_HPP

#include "../token/VnlcToken.hpp"
#include <utility>

class VnlcAstNode {
private:
    VnlcAstNode() = delete;

protected:
    unsigned int offset;
    unsigned int length;

    VnlcToken firstToken;
    VnlcToken lastToken;

    VnlcAstNode(VnlcToken&& firstToken, VnlcToken&& lastToken) noexcept;

public:
    std::pair<int, int> locate() const;
    unsigned int getOffset() const;
    unsigned int getLength() const;
};

#endif // VNLC_AST_NODE_HPP
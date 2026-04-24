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

public:
    std::pair<int, int> locate() const;
    unsigned int getOffset() const;
    unsigned int getLength() const;
};

#endif // VNLC_AST_NODE_HPP
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

    unsigned int line;
    unsigned int column;

    VnlcAstNode(const VnlcToken& firstToken, const VnlcToken& lastToken);

public:
    std::pair<unsigned int, unsigned int> locate() const noexcept;
    unsigned int getOffset() const noexcept;
    unsigned int getLength() const noexcept;

    virtual ~VnlcAstNode() = default;
};

#endif // VNLC_AST_NODE_HPP
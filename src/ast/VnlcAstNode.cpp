#include "VnlcAstNode.hpp"

std::pair<int, int> VnlcAstNode::locate() const {
    return { firstToken.getLine(), firstToken.getColumn() };
}

unsigned int VnlcAstNode::getOffset() const {
    return offset;
}

unsigned int VnlcAstNode::getLength() const {
    return length;
}
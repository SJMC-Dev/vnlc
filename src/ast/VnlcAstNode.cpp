#include "VnlcAstNode.hpp"

VnlcAstNode::VnlcAstNode(const VnlcToken& firstToken, const VnlcToken& lastToken) {
    offset = firstToken.getOffset();
    length = lastToken.getOffset() + lastToken.getLength() - offset;
    line = firstToken.getLine();
    column = firstToken.getColumn();
}

std::pair<unsigned int, unsigned int> VnlcAstNode::locate() const noexcept {
    return { line, column };
}

unsigned int VnlcAstNode::getOffset() const noexcept {
    return offset;
}

unsigned int VnlcAstNode::getLength() const noexcept {
    return length;
}
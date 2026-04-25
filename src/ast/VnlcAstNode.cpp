#include "VnlcAstNode.hpp"

VnlcAstNode::VnlcAstNode(VnlcToken&& firstToken, VnlcToken&& lastToken) noexcept : firstToken(std::move(firstToken)), lastToken(std::move(lastToken)) {
    offset = this->firstToken.getOffset();
    length = this->lastToken.getOffset() + this->lastToken.getLength() - offset;
}

std::pair<int, int> VnlcAstNode::locate() const {
    return { firstToken.getLine(), firstToken.getColumn() };
}

unsigned int VnlcAstNode::getOffset() const {
    return offset;
}

unsigned int VnlcAstNode::getLength() const {
    return length;
}
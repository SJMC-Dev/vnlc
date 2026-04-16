#include "VnlcLexer.hpp"

VnlcLexer::VnlcLexer(std::istream& input) : source(input), currentCharIndex(0), line(0), column(0) {}

inline int VnlcLexer::getCurrentLine() const { return line; }
inline int VnlcLexer::getCurrentColumn() const { return column; }

void VnlcLexer::advance() {
    // TODO
}

VnlcToken VnlcLexer::next() {
    // TODO
}
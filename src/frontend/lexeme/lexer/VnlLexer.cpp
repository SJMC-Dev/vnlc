#include "VnlLexer.hpp"

VnlLexer::VnlLexer(std::istream& input) : source(input), currentCharIndex(0), line(0), column(0) {}

inline int VnlLexer::getCurrentLine() const { return line; }
inline int VnlLexer::getCurrentColumn() const { return column; }

void VnlLexer::advance() {
    // TODO
}

VnlToken VnlLexer::next() {
    // TODO
}
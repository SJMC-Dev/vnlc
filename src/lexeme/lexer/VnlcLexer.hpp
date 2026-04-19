#pragma once

#ifndef VNLC_LEXER_HPP
#define VNLC_LEXER_HPP

#include "../token/VnlcToken.hpp"

#include <istream>

class VnlcLexer {
private:
    std::istream& source;
    int currentCharIndex;

    int line;
    int column;

    void advance();

public:
    VnlcLexer(std::istream& input);

    inline int getCurrentLine() const;
    inline int getCurrentColumn() const;

    VnlcToken next();
};

#endif // VNLC_LEXER_HPP
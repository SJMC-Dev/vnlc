#pragma once

#ifndef __VNLC_LEXER_HPP__
#define __VNLC_LEXER_HPP__

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

#endif // __VNLC_LEXER_HPP__
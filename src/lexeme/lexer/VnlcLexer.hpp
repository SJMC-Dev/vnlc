#pragma once

#ifndef VNLC_LEXER_HPP
#define VNLC_LEXER_HPP

#include "../token/VnlcToken.hpp"

#include <istream>

class VnlcLexer {
private:
    std::istream& source;
    std::string currentLine;

    int line;
    int column;

    bool exhausted;

    void advance();
    bool readline();

    inline bool blank() const;
    inline bool number() const;
    inline bool special() const;
    inline bool newline() const;
    inline bool eof() const;

    inline int peek() const;

public:
    VnlcLexer(std::istream& input);

    bool hasNext();
    VnlcToken next();
};

#endif // VNLC_LEXER_HPP
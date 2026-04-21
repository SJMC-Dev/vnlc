#pragma once

#ifndef VNLC_LEXER_HPP
#define VNLC_LEXER_HPP

#include "../token/VnlcToken.hpp"
#include "VnlcLexerMode.hpp"

#include <istream>
#include <stack>

class VnlcLexer {
private:
    VnlcLexerMode mode;
    std::stack<VnlcLexerMode> modeStack;
    std::stack<int> parenthesisCounterStack;
    int parenthesisCounter;

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

    VnlcToken processStartsWithBlank(std::string& tokenValue, int currentLine, int currentColumn);
    VnlcToken processStartsWithNumber(std::string& tokenValue, int currentLine, int currentColumn);
    VnlcToken processStartsWithSpecial(std::string& tokenValue, int currentLine, int currentColumn);
    VnlcToken processStartsWithNewline(std::string& tokenValue, int currentLine, int currentColumn);
    VnlcToken processStartsWithEof(std::string& tokenValue, int currentLine, int currentColumn);
    VnlcToken processStartsWithIdentifier(std::string& tokenValue, int currentLine, int currentColumn);

public:
    VnlcLexer(std::istream& input);

    bool hasNext();
    VnlcToken next();
};

#endif // VNLC_LEXER_HPP
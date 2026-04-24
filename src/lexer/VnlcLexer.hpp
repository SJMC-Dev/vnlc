#pragma once

#ifndef VNLC_LEXER_HPP
#define VNLC_LEXER_HPP

#include "../token/VnlcToken.hpp"
#include "VnlcLexerMode.hpp"

#include <istream>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>

class VnlcLexer {
private:
    VnlcLexerMode mode;
    std::stack<int> parenthesisCounterStack;
    int parenthesisCounter;

    std::unordered_map<std::string_view, VnlcTokenType> keywords;

    std::istream& source;
    std::string currentLine;

    unsigned int offset;
    int line;
    int column;

    bool exhausted;

    void advance();
    bool readline();
    void collect(std::string& tokenValue);

    bool blank() const;
    bool number() const;
    bool special() const;
    bool newline() const;
    bool eof() const;
    bool separator() const;

    int peek() const;
    int peek(int offset) const;

    VnlcToken processStartsWithBlank(std::string& tokenValue, int currentLine, int currentColumn, unsigned int currentOffset);
    VnlcToken processStartsWithNumber(std::string& tokenValue, int currentLine, int currentColumn, unsigned int currentOffset);
    VnlcToken processStartsWithSpecial(std::string& tokenValue, int currentLine, int currentColumn, unsigned int currentOffset);
    VnlcToken processStartsWithNewline(std::string& tokenValue, int currentLine, int currentColumn, unsigned int currentOffset);
    VnlcToken processStartsWithEof(std::string& tokenValue, int currentLine, int currentColumn, unsigned int currentOffset);
    VnlcToken processStartsWithIdentifier(std::string& tokenValue, int currentLine, int currentColumn, unsigned int currentOffset);

    VnlcToken scanStringLiteral(std::string& tokenValue, int currentLine, int currentColumn, unsigned int currentOffset);
    VnlcToken scanFormatStringLiteral(std::string& tokenValue, int currentLine, int currentColumn, unsigned int currentOffset);
    VnlcToken scanRawStringLiteral(std::string& tokenValue, int currentLine, int currentColumn, unsigned int currentOffset);

public:
    VnlcLexer(std::istream& input);

    bool hasNext();
    VnlcToken next();
};

#endif // VNLC_LEXER_HPP
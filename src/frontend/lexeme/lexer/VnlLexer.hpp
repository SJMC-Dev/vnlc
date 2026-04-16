#pragma once

#ifndef __VNL_LEXER_HPP__
#define __VNL_LEXER_HPP__

#include "../token/VnlToken.hpp"

#include <istream>

class VnlLexer {
private:
    std::istream& source;
    int currentCharIndex;

    int line;
    int column;

    void advance();

public:
    VnlLexer(std::istream& input);

    VnlToken next();
};

#endif // __VNL_LEXER_HPP__
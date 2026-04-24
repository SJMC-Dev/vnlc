#pragma once

#ifndef VNLC_PARSER_HPP
#define VNLC_PARSER_HPP

#include "../lexer/VnlcLexer.hpp"
#include "../token/VnlcToken.hpp"

class VnlcParser {
private:
    VnlcLexer lexer;
    VnlcToken currentToken;
};

#endif // VNLC_PARSER_HPP
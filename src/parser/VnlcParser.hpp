#pragma once

#ifndef VNLC_PARSER_HPP
#define VNLC_PARSER_HPP

#include "../ast/VnlcAstNode.hpp"
#include "../lexer/VnlcLexer.hpp"
#include "../token/VnlcToken.hpp"
#include <memory>

class VnlcParser {
private:
    VnlcLexer lexer;
    VnlcToken currentToken;

public:
    VnlcParser(VnlcLexer&& lexer);

    bool hasNext() const;
    std::unique_ptr<VnlcAstNode> parse();
};

#endif // VNLC_PARSER_HPP
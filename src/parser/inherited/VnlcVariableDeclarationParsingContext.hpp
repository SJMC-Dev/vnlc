#pragma once

#ifndef VNLC_VARIABLE_DECLARATION_PARSING_CONTEXT_HPP
#define VNLC_VARIABLE_DECLARATION_PARSING_CONTEXT_HPP

struct VnlcVariableDeclarationParsingContext {
    enum class Position {
        TOP_LEVEL,
        STATEMENT,
    };

    Position position;
};

#endif // VNLC_VARIABLE_DECLARATION_PARSING_CONTEXT_HPP
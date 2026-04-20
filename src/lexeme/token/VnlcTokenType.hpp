#pragma once

#ifndef VNLC_TOKEN_TYPE_HPP
#define VNLC_TOKEN_TYPE_HPP

enum class VnlcTokenType {
    IDENTIFIER,

    INTEGER,
    BYTE,
    CHAR,
    SHORT,
    LONG,
    FLOAT,
    DOUBLE,
    BINARY,
    OCTAL,
    HEXADECIMAL,

    STRING,
    ARRAY_LIST,
    SNBT_INTEGER_ARRAY,
    SNBT_BYTE_ARRAY,
    SNBT_LONG_ARRAY,
    SNBT_COMPOUND,
    SELECTOR,
    RANGE,
    REGEX,

    VAR,
    LET,
    CONST,

    FUNC,
    RETURN,
    IF,
    ELSE,
    FOR,
    WHILE,
    LABEL,
    BREAK,
    CONTINUE,
    SWITCH,
    CASE,
    DEFAULT,
    WHEN,
    CONTEXT,
    VOID,
    NATIVE,
    CALLEE,
    IN,
    RELOAD,

    CLASS,
    INTERFACE,
    ABSTRACT,
    TYPE,
    ENUM,
    RECORD,
    EXTENDS,
    IMPLEMENTS,
    THIS,
    SUPER,
    PRIVATE,
    PROTECTED,
    PUBLIC,
    READONLY,
    STATIC,
    INSTANCEOF,
    FINAL,
    OVERRIDE,

    TRUE,
    FALSE,

    IMPORT,
    EXPORT,
    AS,
    MODULE,
    PARENT,

    LAMBDA,
    COMMAND,
    ASYNC,
    AWAIT,
    GENERATOR,
    YIELD,
    FROM,
    DECORATOR,
    DECORATE,
    TRY,
    CATCH,
    FINALLY,
    THROW,
    PANIC,
    ASSERT,
    TYPEOF,
    DEFER,

    METADATA,
    DEPRECATED,
    EXPERIMENTAL,
    NOWARNINGS,
    GAMEVERSION,
    GAMEVERSION_RANGE,

    PLUS,                  // +
    MINUS,                 // -
    ASTERISK,              // *
    SLASH,                 // /
    DOUBLE_SLASH,          // //
    PERCENT,               // %
    DOUBLE_ASTERISK,       // **
    PLUS_EQUAL,            // +=
    MINUS_EQUAL,           // -=
    ASTERISK_EQUAL,        // *=
    SLASH_EQUAL,           // /=
    DOUBLE_SLASH_EQUAL,    // //=
    PERCENT_EQUAL,         // %=
    DOUBLE_ASTERISK_EQUAL, // **=

    EQUAL,                 // =
    DOUBLE_QUESTION_EQUAL, // ??=

    DOUBLE_EQUAL,  // ==
    NOT_EQUAL,     // !=
    GREATER,       // >
    GREATER_EQUAL, // >=
    LESS,          // <
    LESS_EQUAL,    // <=

    DOUBLE_AND,      // &&
    DOUBLE_OR,       // ||
    DOUBLE_QUESTION, // ??
    EXCLAMATION,     // !

    ARROW, // ->

    AND,                        // &
    OR,                         // |
    XOR,                        // ^
    BITWISE_NOT,                // ~
    LEFT_SHIFT,                 // <<
    RIGHT_SHIFT,                // >>
    UNSIGNED_RIGHT_SHIFT,       // >>>
    AND_EQUAL,                  // &=
    OR_EQUAL,                   // |=
    XOR_EQUAL,                  // ^=
    LEFT_SHIFT_EQUAL,           // <<=
    RIGHT_SHIFT_EQUAL,          // >>=
    UNSIGNED_RIGHT_SHIFT_EQUAL, // >>>=

    DOT,             // .
    COMMA,           // ,
    SEMICOLON,       // ;
    COLON,           // :
    DOUBLE_COLON,    // ::
    QUESTION,        // ?
    QUESTION_DOT,    // ?.
    EXCLAMATION_DOT, // !.
    DOUBLE_DOT,      // ..
    TRIPLE_DOT,      // ...

    LEFT_PAREN,    // (
    RIGHT_PAREN,   // )
    LEFT_BRACE,    // {
    RIGHT_BRACE,   // }
    LEFT_BRACKET,  // [
    RIGHT_BRACKET, // ]

    SINGLE_LINE_COMMENT, // # comment
    MULTI_LINE_COMMENT,  // #* comment *#

    BLANK,
    NEWLINE,

    LEXICAL_ERROR,

    END_OF_FILE,
};

#endif // VNLC_TOKEN_TYPE_HPP
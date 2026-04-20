#include "VnlcLexer.hpp"
#include "../../error/VnlcOutOfRangeError.hpp"

VnlcLexer::VnlcLexer(std::istream& input) : source(input), currentLine(""), line(-1), column(0), exhausted(false) { readline(); }

inline int VnlcLexer::peek() const {
    if (column < static_cast<int>(currentLine.length())) {
        return static_cast<char>(currentLine[column]);
    }
    return std::char_traits<char>::eof();
}

inline bool VnlcLexer::blank() const {
    int ch = peek();
    if (ch == std::char_traits<char>::eof())
        return false;
    return ch == ' ' || ch == '\t' || ch == '\r';
}

inline bool VnlcLexer::number() const {
    int ch = peek();
    if (ch == std::char_traits<char>::eof())
        return false;
    return ch >= '0' && ch <= '9';
}

inline bool VnlcLexer::special() const {
    int c = peek();
    if (c == std::char_traits<char>::eof())
        return false;

    constexpr std::string_view specialChars = "+-*/=!<>|&^%~?:.,;(){}[]#@$\\`\"'";
    return specialChars.find(static_cast<char>(c)) != std::string_view::npos;
}

inline bool VnlcLexer::newline() const {
    int ch = peek();
    if (ch == std::char_traits<char>::eof())
        return false;
    return ch == '\n';
}

bool VnlcLexer::readline() {
    if (!std::getline(source, currentLine)) {
        return false;
    }
    line++;

    if (source.peek() != std::char_traits<char>::eof()) {
        currentLine += '\n';
    }

    column = 0;
    return true;
}

void VnlcLexer::advance() {
    if (exhausted) {
        throw VnlcOutOfRangeError("No more tokens to read.");
    }

    if (peek() == std::char_traits<char>::eof()) {
        exhausted = true;
        return;
    }

    int len = static_cast<int>(currentLine.length());

    if (column < len - 1) {
        column++;
        return;
    }

    if (readline()) {
        return;
    }

    column = len;
}

bool VnlcLexer::hasNext() { return !exhausted; }

VnlcToken VnlcLexer::next() {
    if (!hasNext()) {
        throw VnlcOutOfRangeError("No more tokens to read.");
    }

    std::string tokenValue;
    int currentLine = line + 1;
    int currentColumn = column + 1;

    if (peek() == std::char_traits<char>::eof()) {
        advance();
        return VnlcToken(VnlcTokenType::END_OF_FILE, "", currentLine, currentColumn);
    }

    if (blank()) {
        while (blank()) {
            char currentChar = static_cast<char>(peek());
            if (currentChar != '\r') {
                tokenValue.push_back(currentChar);
            }
            advance();
        }

        return VnlcToken(VnlcTokenType::BLANK, std::move(tokenValue), currentLine, currentColumn);
    }

    if (newline()) {
        tokenValue.push_back('\n');
        advance();
        return VnlcToken(VnlcTokenType::NEWLINE, std::move(tokenValue), currentLine, currentColumn);
    }

    if (special()) {
        char currentChar = static_cast<char>(peek());
        tokenValue.push_back(currentChar);
        advance();

        char nextChar = static_cast<char>(peek());

        if (currentChar == '+') {
            if (nextChar == '=') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::PLUS_EQUAL, std::move(tokenValue), currentLine, currentColumn);
            } else {
                return VnlcToken(VnlcTokenType::PLUS, std::move(tokenValue), currentLine, currentColumn);
            }
        } else if (currentChar == '-') {
            if (nextChar == '=') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::MINUS_EQUAL, std::move(tokenValue), currentLine, currentColumn);
            } else if (nextChar == '>') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::ARROW, std::move(tokenValue), currentLine, currentColumn);
            } else {
                return VnlcToken(VnlcTokenType::MINUS, std::move(tokenValue), currentLine, currentColumn);
            }
        } else if (currentChar == '*') {
            if (nextChar == '=') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::ASTERISK_EQUAL, std::move(tokenValue), currentLine, currentColumn);
            } else if (nextChar == '*') {
                tokenValue.push_back(nextChar);
                advance();
                nextChar = static_cast<char>(peek());
                if (nextChar == '=') {
                    tokenValue.push_back(nextChar);
                    advance();
                    return VnlcToken(VnlcTokenType::DOUBLE_ASTERISK_EQUAL, std::move(tokenValue), currentLine, currentColumn);
                } else {
                    return VnlcToken(VnlcTokenType::DOUBLE_ASTERISK, std::move(tokenValue), currentLine, currentColumn);
                }
            } else {
                return VnlcToken(VnlcTokenType::ASTERISK, std::move(tokenValue), currentLine, currentColumn);
            }
        } else if (currentChar == '/') {
            if (nextChar == '=') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::SLASH_EQUAL, std::move(tokenValue), currentLine, currentColumn);
            } else if (nextChar == '/') {
                tokenValue.push_back(nextChar);
                advance();
                nextChar = static_cast<char>(peek());
                if (nextChar == '=') {
                    tokenValue.push_back(nextChar);
                    advance();
                    return VnlcToken(VnlcTokenType::DOUBLE_SLASH_EQUAL, std::move(tokenValue), currentLine, currentColumn);
                } else {
                    return VnlcToken(VnlcTokenType::DOUBLE_SLASH, std::move(tokenValue), currentLine, currentColumn);
                }
            } else {
                return VnlcToken(VnlcTokenType::SLASH, std::move(tokenValue), currentLine, currentColumn);
            }
        } else if (currentChar == '%') {
            if (nextChar == '=') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::PERCENT_EQUAL, std::move(tokenValue), currentLine, currentColumn);
            } else {
                return VnlcToken(VnlcTokenType::PERCENT, std::move(tokenValue), currentLine, currentColumn);
            }
        } else if (currentChar == '<') {
            if (nextChar == '=') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::LEFT_ANGLE_EQUAL, std::move(tokenValue), currentLine, currentColumn);
            } else if (nextChar == '<') {
                tokenValue.push_back(nextChar);
                advance();
                nextChar = static_cast<char>(peek());
                if (nextChar == '=') {
                    tokenValue.push_back(nextChar);
                    advance();
                    return VnlcToken(VnlcTokenType::DOUBLE_LEFT_ANGLE_EQUAL, std::move(tokenValue), currentLine, currentColumn);
                } else {
                    return VnlcToken(VnlcTokenType::DOUBLE_LEFT_ANGLE, std::move(tokenValue), currentLine, currentColumn);
                }
            } else {
                return VnlcToken(VnlcTokenType::LEFT_ANGLE, std::move(tokenValue), currentLine, currentColumn);
            }
        } else if (currentChar == '>') {
            if (nextChar == '=') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::RIGHT_ANGLE_EQUAL, std::move(tokenValue), currentLine, currentColumn);
            } else if (nextChar == '>') {
                tokenValue.push_back(nextChar);
                advance();
                nextChar = static_cast<char>(peek());
                if (nextChar == '=') {
                    tokenValue.push_back(nextChar);
                    advance();
                    return VnlcToken(VnlcTokenType::DOUBLE_RIGHT_ANGLE_EQUAL, std::move(tokenValue), currentLine, currentColumn);
                } else if (nextChar == '>') {
                    tokenValue.push_back(nextChar);
                    advance();
                    nextChar = static_cast<char>(peek());
                    if (nextChar == '=') {
                        tokenValue.push_back(nextChar);
                        advance();
                        return VnlcToken(VnlcTokenType::TRIPLE_RIGHT_ANGLE_EQUAL, std::move(tokenValue), currentLine, currentColumn);
                    } else {
                        return VnlcToken(VnlcTokenType::TRIPLE_RIGHT_ANGLE, std::move(tokenValue), currentLine, currentColumn);
                    }
                } else {
                    return VnlcToken(VnlcTokenType::DOUBLE_RIGHT_ANGLE, std::move(tokenValue), currentLine, currentColumn);
                }
            } else {
                return VnlcToken(VnlcTokenType::RIGHT_ANGLE, std::move(tokenValue), currentLine, currentColumn);
            }
        } else if (currentChar == '=') {
            if (nextChar == '=') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::DOUBLE_EQUAL, std::move(tokenValue), currentLine, currentColumn);
            } else {
                return VnlcToken(VnlcTokenType::EQUAL, std::move(tokenValue), currentLine, currentColumn);
            }
        } else if (currentChar == '?') {
            if (nextChar == '?') {
                tokenValue.push_back(nextChar);
                advance();
                nextChar = static_cast<char>(peek());
                if (nextChar == '=') {
                    tokenValue.push_back(nextChar);
                    advance();
                    return VnlcToken(VnlcTokenType::DOUBLE_QUESTION_EQUAL, std::move(tokenValue), currentLine, currentColumn);
                } else {
                    return VnlcToken(VnlcTokenType::DOUBLE_QUESTION, std::move(tokenValue), currentLine, currentColumn);
                }
            } else if (nextChar == '.') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::QUESTION_DOT, std::move(tokenValue), currentLine, currentColumn);
            } else {
                return VnlcToken(VnlcTokenType::QUESTION, std::move(tokenValue), currentLine, currentColumn);
            }
        } else if (currentChar == '!') {
            if (nextChar == '=') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::EXCLAMATION_EQUAL, std::move(tokenValue), currentLine, currentColumn);
            } else if (nextChar == '.') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::EXCLAMATION_DOT, std::move(tokenValue), currentLine, currentColumn);
            } else {
                return VnlcToken(VnlcTokenType::EXCLAMATION, std::move(tokenValue), currentLine, currentColumn);
            }
        } else if (currentChar == '&') {
            if (nextChar == '&') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::DOUBLE_AMPERSAND, std::move(tokenValue), currentLine, currentColumn);
            } else if (nextChar == '=') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::AMPERSAND_EQUAL, std::move(tokenValue), currentLine, currentColumn);
            } else {
                return VnlcToken(VnlcTokenType::AMPERSAND, std::move(tokenValue), currentLine, currentColumn);
            }
        } else if (currentChar == '|') {
            if (nextChar == '|') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::DOUBLE_PIPE, std::move(tokenValue), currentLine, currentColumn);
            } else if (nextChar == '=') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::PIPE_EQUAL, std::move(tokenValue), currentLine, currentColumn);
            } else {
                return VnlcToken(VnlcTokenType::PIPE, std::move(tokenValue), currentLine, currentColumn);
            }
        } else if (currentChar == '^') {
            if (nextChar == '=') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::CARET_EQUAL, std::move(tokenValue), currentLine, currentColumn);
            } else {
                return VnlcToken(VnlcTokenType::CARET, std::move(tokenValue), currentLine, currentColumn);
            }
        } else if (currentChar == '~') {
            return VnlcToken(VnlcTokenType::TILDE, std::move(tokenValue), currentLine, currentColumn);
        } else if (currentChar == '.') {
            if (nextChar == '.') {
                tokenValue.push_back(nextChar);
                advance();
                nextChar = static_cast<char>(peek());
                if (nextChar == '.') {
                    tokenValue.push_back(nextChar);
                    advance();
                    return VnlcToken(VnlcTokenType::TRIPLE_DOT, std::move(tokenValue), currentLine, currentColumn);
                } else {
                    return VnlcToken(VnlcTokenType::DOUBLE_DOT, std::move(tokenValue), currentLine, currentColumn);
                }
            } else {
                return VnlcToken(VnlcTokenType::DOT, std::move(tokenValue), currentLine, currentColumn);
            }
        } else if (currentChar == ',') {
            return VnlcToken(VnlcTokenType::COMMA, std::move(tokenValue), currentLine, currentColumn);
        } else if (currentChar == ';') {
            return VnlcToken(VnlcTokenType::SEMICOLON, std::move(tokenValue), currentLine, currentColumn);
        } else if (currentChar == ':') {
            if (nextChar == ':') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::DOUBLE_COLON, std::move(tokenValue), currentLine, currentColumn);
            } else {
                return VnlcToken(VnlcTokenType::COLON, std::move(tokenValue), currentLine, currentColumn);
            }
        } else if (currentChar == '(') {
            return VnlcToken(VnlcTokenType::LEFT_PARENTHESIS, std::move(tokenValue), currentLine, currentColumn);
        } else if (currentChar == ')') {
            return VnlcToken(VnlcTokenType::RIGHT_PARENTHESIS, std::move(tokenValue), currentLine, currentColumn);
        } else if (currentChar == '{') {
            return VnlcToken(VnlcTokenType::LEFT_BRACE, std::move(tokenValue), currentLine, currentColumn);
        } else if (currentChar == '}') {
            return VnlcToken(VnlcTokenType::RIGHT_BRACE, std::move(tokenValue), currentLine, currentColumn);
        } else if (currentChar == '[') {
            return VnlcToken(VnlcTokenType::LEFT_BRACKET, std::move(tokenValue), currentLine, currentColumn);
        } else if (currentChar == ']') {
            return VnlcToken(VnlcTokenType::RIGHT_BRACKET, std::move(tokenValue), currentLine, currentColumn);
        }
    }

    return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
}
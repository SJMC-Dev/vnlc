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
    if (eof()) {
        return false;
    }
    char ch = static_cast<char>(peek());
    return ch == ' ' || ch == '\t' || ch == '\r';
}

inline bool VnlcLexer::number() const {
    if (eof()) {
        return false;
    }
    char ch = static_cast<char>(peek());
    return ch >= '0' && ch <= '9';
}

inline bool VnlcLexer::special() const {
    if (eof()) {
        return false;
    }
    char c = static_cast<char>(peek());
    constexpr std::string_view specialChars = "+-*/=!<>|&^%~?:.,;(){}[]#@$\\`\"'";
    return specialChars.find(c) != std::string_view::npos;
}

inline bool VnlcLexer::newline() const {
    if (eof()) {
        return false;
    }
    char ch = static_cast<char>(peek());
    return ch == '\n';
}

inline bool VnlcLexer::eof() const { return peek() == std::char_traits<char>::eof(); }

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

    if (eof()) {
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

VnlcToken VnlcLexer::processStartsWithBlank(std::string& tokenValue, int currentLine, int currentColumn) {
    while (blank()) {
        char currentChar = static_cast<char>(peek());
        if (currentChar != '\r') {
            tokenValue.push_back(currentChar);
        }
        advance();
    }

    return VnlcToken(VnlcTokenType::BLANK, std::move(tokenValue), currentLine, currentColumn);
}

VnlcToken VnlcLexer::processStartsWithSpecial(std::string& tokenValue, int currentLine, int currentColumn) {
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
    } else if (currentChar == '#') {
        if (nextChar == '*') {
            tokenValue.push_back(nextChar);
            advance();
            while (true) {
                if (eof()) {
                    return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
                }

                char c = static_cast<char>(peek());
                tokenValue.push_back(c);
                advance();

                if (c == '*' && peek() == '#') {
                    tokenValue.push_back(static_cast<char>(peek()));
                    advance();
                    break;
                }
            }

            return VnlcToken(VnlcTokenType::MULTI_LINE_COMMENT, std::move(tokenValue), currentLine, currentColumn);
        } else {
            while (!eof() && !newline()) {
                tokenValue.push_back(static_cast<char>(peek()));
                advance();
            }
            return VnlcToken(VnlcTokenType::SINGLE_LINE_COMMENT, std::move(tokenValue), currentLine, currentColumn);
        }
    } else if (currentChar == '@') {
        if (nextChar == 'p' || nextChar == 'r' || nextChar == 'a' || nextChar == 'e' || nextChar == 's' || nextChar == 'n') {
            tokenValue.push_back(nextChar);
            advance();
            return VnlcToken(VnlcTokenType::SELECTOR_PREFIX, std::move(tokenValue), currentLine, currentColumn);
        } else {
            return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
        }
    } else if (currentChar == '$') {
        if (nextChar == '(') {
            tokenValue.push_back(nextChar);
            advance();
            return VnlcToken(VnlcTokenType::INTERPOLATION_START, std::move(tokenValue), currentLine, currentColumn);
        } else {
            return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
        }
    } else if (currentChar == '\'') {
        std::string_view asciiChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&()*+,-./:;<=>?@[\\]^_`{|}~ ";
        if (nextChar == '\\') {
            tokenValue.push_back(nextChar);
            advance();
            nextChar = static_cast<char>(peek());
            std::string_view escapeChars = "bfnrst\\'";
            if (escapeChars.find(nextChar) != std::string_view::npos) {
                tokenValue.push_back(nextChar);
                advance();
                nextChar = static_cast<char>(peek());
                if (nextChar == '\'') {
                    tokenValue.push_back(nextChar);
                    advance();
                    return VnlcToken(VnlcTokenType::CHAR, std::move(tokenValue), currentLine, currentColumn);
                } else {
                    return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
                }
            } else {
                return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
            }
        } else if (asciiChars.find(nextChar) != std::string_view::npos) {
            tokenValue.push_back(nextChar);
            advance();
            nextChar = static_cast<char>(peek());
            if (nextChar == '\'') {
                tokenValue.push_back(nextChar);
                advance();
                return VnlcToken(VnlcTokenType::CHAR, std::move(tokenValue), currentLine, currentColumn);
            } else {
                return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
            }
        } else {
            return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
        }
    } else {
        return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
    }
}

VnlcToken VnlcLexer::processStartsWithNewline(std::string& tokenValue, int currentLine, int currentColumn) {
    tokenValue.push_back('\n');
    advance();
    return VnlcToken(VnlcTokenType::NEWLINE, std::move(tokenValue), currentLine, currentColumn);
}

VnlcToken VnlcLexer::processStartsWithEof(std::string& tokenValue, int currentLine, int currentColumn) {
    advance();
    return VnlcToken(VnlcTokenType::END_OF_FILE, "", currentLine, currentColumn);
}

VnlcToken VnlcLexer::next() {
    if (!hasNext()) {
        throw VnlcOutOfRangeError("No more tokens to read.");
    }

    std::string tokenValue;
    int currentLine = line + 1;
    int currentColumn = column + 1;

    if (eof()) {
        return processStartsWithEof(tokenValue, currentLine, currentColumn);
    } else if (blank()) {
        return processStartsWithBlank(tokenValue, currentLine, currentColumn);
    } else if (newline()) {
        return processStartsWithNewline(tokenValue, currentLine, currentColumn);
    } else if (special()) {
        return processStartsWithSpecial(tokenValue, currentLine, currentColumn);
    }

    return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
}
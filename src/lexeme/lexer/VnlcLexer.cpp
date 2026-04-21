#include "VnlcLexer.hpp"
#include "../../error/VnlcOutOfRangeError.hpp"
#include <string>
#include <string_view>

VnlcLexer::VnlcLexer(std::istream& input)
    : mode(VnlcLexerMode::DEFAULT), parenthesisCounterStack(), parenthesisCounter(-1), source(input), currentLine(""), line(-1), column(0), exhausted(false), keywords{
          { "var", VnlcTokenType::VAR },
          { "let", VnlcTokenType::LET },
          { "const", VnlcTokenType::CONST },
          { "func", VnlcTokenType::FUNC },
          { "return", VnlcTokenType::RETURN },
          { "if", VnlcTokenType::IF },
          { "else", VnlcTokenType::ELSE },
          { "for", VnlcTokenType::FOR },
          { "while", VnlcTokenType::WHILE },
          { "label", VnlcTokenType::LABEL },
          { "break", VnlcTokenType::BREAK },
          { "continue", VnlcTokenType::CONTINUE },
          { "switch", VnlcTokenType::SWITCH },
          { "case", VnlcTokenType::CASE },
          { "default", VnlcTokenType::DEFAULT },
          { "when", VnlcTokenType::WHEN },
          { "context", VnlcTokenType::CONTEXT },
          { "void", VnlcTokenType::VOID },
          { "native", VnlcTokenType::NATIVE },
          { "callee", VnlcTokenType::CALLEE },
          { "in", VnlcTokenType::IN },
          { "reload", VnlcTokenType::RELOAD },
          { "class", VnlcTokenType::CLASS },
          { "interface", VnlcTokenType::INTERFACE },
          { "abstract", VnlcTokenType::ABSTRACT },
          { "type", VnlcTokenType::TYPE },
          { "enum", VnlcTokenType::ENUM },
          { "record", VnlcTokenType::RECORD },
          { "extends", VnlcTokenType::EXTENDS },
          { "implements", VnlcTokenType::IMPLEMENTS },
          { "this", VnlcTokenType::THIS },
          { "super", VnlcTokenType::SUPER },
          { "private", VnlcTokenType::PRIVATE },
          { "protected", VnlcTokenType::PROTECTED },
          { "public", VnlcTokenType::PUBLIC },
          { "readonly", VnlcTokenType::READONLY },
          { "static", VnlcTokenType::STATIC },
          { "instanceof", VnlcTokenType::INSTANCEOF },
          { "final", VnlcTokenType::FINAL },
          { "override", VnlcTokenType::OVERRIDE },
          { "none", VnlcTokenType::NONE },
          { "true", VnlcTokenType::TRUE },
          { "false", VnlcTokenType::FALSE },
          { "import", VnlcTokenType::IMPORT },
          { "export", VnlcTokenType::EXPORT },
          { "as", VnlcTokenType::AS },
          { "module", VnlcTokenType::MODULE },
          { "parent", VnlcTokenType::PARENT },
          { "self", VnlcTokenType::SELF },
          { "lambda", VnlcTokenType::LAMBDA },
          { "command", VnlcTokenType::COMMAND },
          { "async", VnlcTokenType::ASYNC },
          { "await", VnlcTokenType::AWAIT },
          { "generator", VnlcTokenType::GENERATOR },
          { "yield", VnlcTokenType::YIELD },
          { "from", VnlcTokenType::FROM },
          { "decorator", VnlcTokenType::DECORATOR },
          { "decorate", VnlcTokenType::DECORATE },
          { "try", VnlcTokenType::TRY },
          { "catch", VnlcTokenType::CATCH },
          { "finally", VnlcTokenType::FINALLY },
          { "throw", VnlcTokenType::THROW },
          { "panic", VnlcTokenType::PANIC },
          { "assert", VnlcTokenType::ASSERT },
          { "typeof", VnlcTokenType::TYPEOF },
          { "defer", VnlcTokenType::DEFER },
          { "asset", VnlcTokenType::ASSET },
          { "define", VnlcTokenType::DEFINE },
          { "metadata", VnlcTokenType::METADATA },
          { "deprecated", VnlcTokenType::DEPRECATED },
          { "experimental", VnlcTokenType::EXPERIMENTAL },
          { "nowarnings", VnlcTokenType::NOWARNINGS },
          { "gameversion", VnlcTokenType::GAMEVERSION },
      } {
    readline();
}

inline int VnlcLexer::peek() const {
    return peek(0);
}

inline int VnlcLexer::peek(int offset) const {
    int targetColumn = column + offset;
    if (targetColumn < static_cast<int>(currentLine.length())) {
        return static_cast<char>(currentLine[targetColumn]);
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

inline bool VnlcLexer::eof() const {
    return peek() == std::char_traits<char>::eof();
}

inline bool VnlcLexer::separator() const {
    return eof() || blank() || special() || newline();
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

bool VnlcLexer::hasNext() {
    return !exhausted;
}

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

VnlcToken VnlcLexer::processStartsWithNumber(std::string& tokenValue, int currentLine, int currentColumn) {
    char currentChar = static_cast<char>(peek());

    constexpr unsigned char BIN = 0b001;
    constexpr unsigned char OCT = 0b010;
    constexpr unsigned char HEX = 0b100;

    bool existE = false;
    bool existDot = mode == VnlcLexerMode::NUMBER_STARTS_WITH_DOT;
    bool nonDecimal = false;
    unsigned char baseFlags = 0;

    mode = VnlcLexerMode::DEFAULT;

    std::string_view hexDigits = "0123456789abcdefABCDEF";
    std::string_view octalDigits = "01234567";
    std::string_view binaryDigits = "01";
    std::string_view nonDecimalFlags = "xob";
    std::string_view exponentFlags = "eE";
    std::string_view integerSuffixes = "bBsSlL";
    std::string_view floatSuffixes = "fFdD";

    while (true) {
        tokenValue.push_back(currentChar);
        advance();
        currentChar = static_cast<char>(peek());

        bool isDot = (currentChar == '.');

        if (tokenValue.size() == 1 && tokenValue.back() == '0' && nonDecimalFlags.find(currentChar) != std::string_view::npos) {
            nonDecimal = true;
            baseFlags = currentChar == 'b' ? BIN : currentChar == 'o' ? OCT : currentChar == 'x' ? HEX : 0;
            tokenValue.push_back(currentChar);
            advance();
            currentChar = static_cast<char>(peek());
        }

        if (isDot) {
            if (peek(1) == '.') {
                break;
            }

            if (nonDecimal || existE || existDot) {
                tokenValue.push_back(currentChar);
                advance();
                currentChar = static_cast<char>(peek());
                while (!separator()) {
                    tokenValue.push_back(currentChar);
                    advance();
                    currentChar = static_cast<char>(peek());
                }
                return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
            }

            existDot = true;
        }

        if (!nonDecimal && exponentFlags.find(currentChar) != std::string_view::npos) {
            tokenValue.push_back(currentChar);
            advance();
            currentChar = static_cast<char>(peek());

            if (existE) {
                while (!separator()) {
                    tokenValue.push_back(currentChar);
                    advance();
                    currentChar = static_cast<char>(peek());
                }
                return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
            }

            if (currentChar == '+' || currentChar == '-') {
                tokenValue.push_back(currentChar);
                advance();
                currentChar = static_cast<char>(peek());

                if (!number()) {
                    while (!separator()) {
                        tokenValue.push_back(currentChar);
                        advance();
                        currentChar = static_cast<char>(peek());
                    }
                    return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
                }
            } else if (!number()) {
                while (!separator()) {
                    tokenValue.push_back(currentChar);
                    advance();
                    currentChar = static_cast<char>(peek());
                }
                return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
            }

            existE = true;
            continue;
        }

        if (nonDecimal) {
            bool error = false;
            error |= baseFlags == BIN && binaryDigits.find(currentChar) == std::string_view::npos;
            error |= baseFlags == OCT && octalDigits.find(currentChar) == std::string_view::npos;
            error |= baseFlags == HEX && hexDigits.find(currentChar) == std::string_view::npos;

            if (error) {
                while (!separator()) {
                    tokenValue.push_back(currentChar);
                    advance();
                    currentChar = static_cast<char>(peek());
                }
                return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
            }
        };

        if (!nonDecimal && !number() && !isDot) {
            if (existDot || existE) {
                if (floatSuffixes.find(currentChar) != std::string_view::npos) {
                    tokenValue.push_back(currentChar);
                    advance();
                    break;
                } else {
                    while (!separator()) {
                        tokenValue.push_back(currentChar);
                        advance();
                        currentChar = static_cast<char>(peek());
                    }
                    return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
                }
            } else {
                if (integerSuffixes.find(currentChar) != std::string_view::npos) {
                    tokenValue.push_back(currentChar);
                    advance();
                    break;
                } else {
                    while (!separator()) {
                        tokenValue.push_back(currentChar);
                        advance();
                        currentChar = static_cast<char>(peek());
                    }
                    return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
                }
            }
        }

        if (!isDot && separator()) {
            break;
        }
    }

    return VnlcToken(VnlcTokenType::NUMBER, std::move(tokenValue), currentLine, currentColumn);
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
        } else if (number()) {
            mode = VnlcLexerMode::NUMBER_STARTS_WITH_DOT;
            return processStartsWithNumber(tokenValue, currentLine, currentColumn);
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
        if (parenthesisCounter >= 0) {
            parenthesisCounter++;
        }
        return VnlcToken(VnlcTokenType::LEFT_PARENTHESIS, std::move(tokenValue), currentLine, currentColumn);
    } else if (currentChar == ')') {
        if (parenthesisCounter > 0) {
            parenthesisCounter--;
            if (parenthesisCounter == 0) {
                parenthesisCounter = parenthesisCounterStack.top();
                parenthesisCounterStack.pop();

                mode = VnlcLexerMode::FORMAT_STRING;
            }
        }
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
            while (!separator()) {
                tokenValue.push_back(nextChar);
                advance();
                nextChar = static_cast<char>(peek());
            }
            return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
        }
    } else if (currentChar == '$') {
        if (mode == VnlcLexerMode::INTERPOLATION_BEGIN && nextChar == '(') {
            tokenValue.push_back(nextChar);
            advance();

            parenthesisCounterStack.push(parenthesisCounter);
            parenthesisCounter = 1;
            mode = VnlcLexerMode::DEFAULT;

            return VnlcToken(VnlcTokenType::INTERPOLATION_START, std::move(tokenValue), currentLine, currentColumn);
        } else {
            while (!separator()) {
                tokenValue.push_back(nextChar);
                advance();
                nextChar = static_cast<char>(peek());
            }
            return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
        }
    } else if (currentChar == '\'') {
        bool error = false;
        int charCount = 0;

        while (true) {
            if (eof() || newline()) {
                return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
            }

            char c = static_cast<char>(peek());
            tokenValue.push_back(c);
            advance();

            if (c == '\\') {
                c = static_cast<char>(peek());
                std::string_view escapeChars = "0bfnrst\\\"'";
                tokenValue.push_back(c);
                advance();
                if (escapeChars.find(c) == std::string_view::npos) {
                    error = true;
                }

                charCount++;
                continue;
            }

            if (c == '\'') {
                break;
            }

            charCount++;
        }

        error |= (charCount != 1);
        if (error) {
            return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
        }
        return VnlcToken(VnlcTokenType::CHAR, std::move(tokenValue), currentLine, currentColumn);
    } else if (currentChar == '"') {
        mode = VnlcLexerMode::STRING;
        return scanStringLiteral(tokenValue, currentLine, currentColumn);
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

VnlcToken VnlcLexer::processStartsWithIdentifier(std::string& tokenValue, int currentLine, int currentColumn) {
    bool error = false;

    if ((peek() == 'f' || peek() == 'F') && peek(1) == '"') {
        tokenValue.push_back(static_cast<char>(peek()));
        advance();
        tokenValue.push_back(static_cast<char>(peek()));
        advance();

        mode = VnlcLexerMode::FORMAT_STRING;
        return scanFormatStringLiteral(tokenValue, currentLine, currentColumn);
    }

    if ((peek() == 'r' || peek() == 'R') && peek(1) == '"') {
        tokenValue.push_back(static_cast<char>(peek()));
        advance();
        tokenValue.push_back(static_cast<char>(peek()));
        advance();

        mode = VnlcLexerMode::RAW_STRING;
        return scanRawStringLiteral(tokenValue, currentLine, currentColumn);
    }

    while (!eof() && !blank() && !special() && !newline()) {
        char c = static_cast<char>(peek());
        tokenValue.push_back(static_cast<char>(c));
        advance();

        if (c >= 0 && c < 32) {
            error = true;
            break;
        }
    }

    if (error) {
        char c = static_cast<char>(peek());
        while (!separator()) {
            tokenValue.push_back(c);
            advance();
            c = static_cast<char>(peek());
        }
        return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
    } else if (keywords.find(tokenValue) != keywords.end()) {
        return VnlcToken(keywords[tokenValue], std::move(tokenValue), currentLine, currentColumn);
    } else {
        return VnlcToken(VnlcTokenType::IDENTIFIER, std::move(tokenValue), currentLine, currentColumn);
    }
}

VnlcToken VnlcLexer::scanStringLiteral(std::string& tokenValue, int currentLine, int currentColumn) {
    bool error = false;

    while (true) {
        if (eof() || newline()) {
            return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
        }

        char currentChar = static_cast<char>(peek());
        tokenValue.push_back(currentChar);
        advance();

        if (currentChar == '\\') {
            currentChar = static_cast<char>(peek());
            std::string_view escapeChars = "bfnrst\\\"'";
            tokenValue.push_back(currentChar);
            advance();
            if (escapeChars.find(currentChar) == std::string_view::npos) {
                error = true;
            }

            continue;
        }

        if (currentChar == '"') {
            break;
        }
    }

    mode = VnlcLexerMode::DEFAULT;
    if (error) {
        return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
    }
    return VnlcToken(VnlcTokenType::STRING, std::move(tokenValue), currentLine, currentColumn);
}

VnlcToken VnlcLexer::scanFormatStringLiteral(std::string& tokenValue, int currentLine, int currentColumn) {
    bool error = false;

    while (true) {
        if (eof() || newline()) {
            return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
        }

        char currentChar = static_cast<char>(peek());
        tokenValue.push_back(currentChar);
        advance();

        if (currentChar == '\\') {
            currentChar = static_cast<char>(peek());
            std::string_view escapeChars = "bfnrst\\\"'$";
            tokenValue.push_back(currentChar);
            advance();
            if (escapeChars.find(currentChar) == std::string_view::npos) {
                error = true;
            }

            continue;
        }

        if (currentChar == '"') {
            mode = VnlcLexerMode::DEFAULT;
            break;
        }

        if (peek() == '$' && peek(1) == '(') {
            mode = VnlcLexerMode::INTERPOLATION_BEGIN;
            break;
        }
    }

    if (error) {
        return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
    }
    return VnlcToken(VnlcTokenType::STRING, std::move(tokenValue), currentLine, currentColumn);
}

VnlcToken VnlcLexer::scanRawStringLiteral(std::string& tokenValue, int currentLine, int currentColumn) {
    while (true) {
        if (eof() || newline()) {
            return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
        }

        char currentChar = static_cast<char>(peek());
        tokenValue.push_back(currentChar);
        advance();

        if (currentChar == '"') {
            break;
        }
    }

    mode = VnlcLexerMode::DEFAULT;
    return VnlcToken(VnlcTokenType::STRING, std::move(tokenValue), currentLine, currentColumn);
}

VnlcToken VnlcLexer::next() {
    if (!hasNext()) {
        throw VnlcOutOfRangeError("No more tokens to read.");
    }

    std::string tokenValue;
    int currentLine = line + 1;
    int currentColumn = column + 1;

    if (mode == VnlcLexerMode::FORMAT_STRING) {
        return scanFormatStringLiteral(tokenValue, currentLine, currentColumn);
    } else {
        if (eof()) {
            return processStartsWithEof(tokenValue, currentLine, currentColumn);
        } else if (blank()) {
            return processStartsWithBlank(tokenValue, currentLine, currentColumn);
        } else if (newline()) {
            return processStartsWithNewline(tokenValue, currentLine, currentColumn);
        } else if (special()) {
            return processStartsWithSpecial(tokenValue, currentLine, currentColumn);
        } else if (number()) {
            return processStartsWithNumber(tokenValue, currentLine, currentColumn);
        } else {
            if (peek() >= 0 && peek() < 32) {
                char c = static_cast<char>(peek());
                while (!separator()) {
                    tokenValue.push_back(c);
                    advance();
                    c = static_cast<char>(peek());
                }
                return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
            }
            return processStartsWithIdentifier(tokenValue, currentLine, currentColumn);
        }
    }

    return VnlcToken(VnlcTokenType::LEXICAL_ERROR, std::move(tokenValue), currentLine, currentColumn);
}
#include "VnlcToken.hpp"

VnlcToken::VnlcToken(VnlcTokenType type, const std::string& value, int line, int column) : type(type), value(value), line(line), column(column) {}

VnlcTokenType VnlcToken::getType() const {
    return type;
}
int VnlcToken::getLine() const {
    return line;
}
int VnlcToken::getColumn() const {
    return column;
}
int VnlcToken::getLength() const {
    return static_cast<int>(value.length());
}
std::string_view VnlcToken::getValue() const {
    return value;
}
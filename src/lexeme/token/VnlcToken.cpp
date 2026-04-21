#include "VnlcToken.hpp"

VnlcToken::VnlcToken(VnlcTokenType type, const std::string& value, int line, int column) : type(type), value(value), line(line), column(column) {}

inline VnlcTokenType VnlcToken::getType() const {
    return type;
}
inline int VnlcToken::getLine() const {
    return line;
}
inline int VnlcToken::getColumn() const {
    return column;
}
inline int VnlcToken::getLength() const {
    return static_cast<int>(value.length());
}
inline std::string_view VnlcToken::getValue() const {
    return value;
}
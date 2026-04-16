#include "VnlcToken.hpp"

VnlcToken::VnlcToken(VnlcTokenType type, const std::string& value, int line, int column) : type(type), value(value), line(line), column(column) {}

inline int VnlcToken::getLine() const { return line; }
inline int VnlcToken::getColumn() const { return column; }
inline std::string_view VnlcToken::getValue() const { return value; }
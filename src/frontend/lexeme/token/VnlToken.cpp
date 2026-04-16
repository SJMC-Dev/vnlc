#include "VnlToken.hpp"

VnlToken::VnlToken(VnlTokenType type, const std::string& value, int line, int column) : type(type), value(value), line(line), column(column) {}

inline int VnlToken::getLine() const { return line; }
inline int VnlToken::getColumn() const { return column; }
inline std::string_view VnlToken::getValue() const { return value; }
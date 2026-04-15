#include "VnlToken.hpp"

VnlToken::VnlToken(VnlTokenType type, const std::string& value, int line, int column) : type(type), value(value), line(line), column(column) {}
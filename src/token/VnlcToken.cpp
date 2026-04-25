#include "VnlcToken.hpp"

VnlcToken::VnlcToken(VnlcTokenType type, const std::string& value, int offset, int line, int column) : type(type), value(value), offset(offset), line(line), column(column) {}
VnlcToken::VnlcToken(VnlcToken&& other) noexcept : type(other.type), value(std::move(other.value)), offset(other.offset), line(other.line), column(other.column) {}

VnlcTokenType VnlcToken::getType() const {
    return type;
}

int VnlcToken::getLine() const {
    return line;
}

int VnlcToken::getColumn() const {
    return column;
}

int VnlcToken::getOffset() const {
    return offset;
}

int VnlcToken::getLength() const {
    return static_cast<int>(value.length());
}

std::string_view VnlcToken::getValue() const {
    return value;
}
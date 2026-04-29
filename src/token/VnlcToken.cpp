#include "VnlcToken.hpp"

VnlcToken::VnlcToken(VnlcTokenType type, std::string_view value, unsigned int offset, unsigned int line, unsigned int column)
    : type(type),
      value(value),
      offset(offset),
      line(line),
      column(column) {}
VnlcToken::VnlcToken(VnlcToken&& other) noexcept : type(other.type), value(std::move(other.value)), offset(other.offset), line(other.line), column(other.column) {}

VnlcTokenType VnlcToken::getType() const noexcept {
    return type;
}

unsigned int VnlcToken::getLine() const noexcept {
    return line;
}

unsigned int VnlcToken::getColumn() const noexcept {
    return column;
}

unsigned int VnlcToken::getOffset() const noexcept {
    return offset;
}

unsigned int VnlcToken::getLength() const noexcept {
    return static_cast<unsigned int>(value.length());
}

std::string_view VnlcToken::getValue() const noexcept {
    return value;
}
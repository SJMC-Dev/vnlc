#pragma once

#ifndef VNLC_TOKEN_HPP
#define VNLC_TOKEN_HPP

#include "VnlcTokenType.hpp"

#include <string>
#include <string_view>

class VnlcToken {
private:
    VnlcTokenType type;
    std::string value;
    unsigned int offset;
    unsigned int line;
    unsigned int column;

public:
    VnlcToken(VnlcTokenType type, std::string_view value, unsigned int offset, unsigned int line, unsigned int column);
    VnlcToken(VnlcToken&& other) noexcept;

    VnlcTokenType getType() const noexcept;
    unsigned int getLine() const noexcept;
    unsigned int getColumn() const noexcept;
    unsigned int getOffset() const noexcept;
    unsigned int getLength() const noexcept;
    std::string_view getValue() const noexcept;
};

#endif // VNLC_TOKEN_HPP
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
    int line;
    int column;

public:
    VnlcToken(VnlcTokenType type, const std::string& value, int line, int column);

    VnlcTokenType getType() const;
    int getLine() const;
    int getColumn() const;
    int getLength() const;
    std::string_view getValue() const;
};

#endif // VNLC_TOKEN_HPP
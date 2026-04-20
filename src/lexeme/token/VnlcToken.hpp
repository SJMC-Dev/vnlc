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

    inline VnlcTokenType getType() const;
    inline int getLine() const;
    inline int getColumn() const;
    inline int getLength() const;
    inline std::string_view getValue() const;
};

#endif // VNLC_TOKEN_HPP
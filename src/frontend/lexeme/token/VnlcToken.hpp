#pragma once

#ifndef __VNLC_TOKEN_HPP__
#define __VNLC_TOKEN_HPP__

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

    inline int getLine() const;
    inline int getColumn() const;
    inline std::string_view getValue() const;
};

#endif // __VNLC_TOKEN_HPP__
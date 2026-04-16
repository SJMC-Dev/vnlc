#pragma once

#ifndef __VNL_TOKEN_HPP__
#define __VNL_TOKEN_HPP__

#include "VnlTokenType.hpp"

#include <string>
#include <string_view>

class VnlToken {
private:
    VnlTokenType type;
    std::string value;
    int line;
    int column;

public:
    VnlToken(VnlTokenType type, const std::string& value, int line, int column);

    inline int getLine() const;
    inline int getColumn() const;
    inline std::string_view getValue() const;
};

#endif // __VNL_TOKEN_HPP__
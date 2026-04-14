#pragma once

#ifndef __VNL_TOKEN_HPP__
#define __VNL_TOKEN_HPP__

#include "VnlTokenType.hpp"

#include <string>

class VnlToken {
public:
    VnlTokenType type;
    std::string value;
    int line;
    int column;

    VnlToken(VnlTokenType type, const std::string& value, int line, int column);
};

#endif // __VNL_TOKEN_HPP__
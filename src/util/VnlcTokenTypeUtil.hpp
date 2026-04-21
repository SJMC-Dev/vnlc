#pragma once

#ifndef VNLC_TOKEN_TYPE_UTIL_HPP
#define VNLC_TOKEN_TYPE_UTIL_HPP

#include "../lexeme/token/VnlcTokenType.hpp"
#include <string_view>

namespace VnlcTokenTypeUtil {
    std::string_view toString(VnlcTokenType type);
}

#endif // VNLC_TOKEN_TYPE_UTIL_HPP
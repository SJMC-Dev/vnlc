#pragma once

#ifndef VNLC_SYNTAX_ERROR_HPP
#define VNLC_SYNTAX_ERROR_HPP

#include "VnlcError.hpp"
#include <fmt/core.h>

class VnlcSyntaxError : public VnlcError {
public:
    VnlcSyntaxError(const std::string& message, int line, int column) : VnlcError(fmt::format("Syntax error at line {}, column {}: {}", line, column, message)) {}
};

#endif // VNLC_SYNTAX_ERROR_HPP
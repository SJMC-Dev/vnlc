#ifndef VNLC_ILLEGAL_INPUT_ERROR_HPP
#define VNLC_ILLEGAL_INPUT_ERROR_HPP

#include "VnlcError.hpp"
#include <fmt/core.h>

class VnlcIllegalInputError : public VnlcError {
public:
    VnlcIllegalInputError(std::string_view message) : VnlcError(fmt::format("Illegal input: {}", message)) {}
};

#endif // VNLC_ILLEGAL_INPUT_ERROR_HPP
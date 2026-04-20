#pragma once

#ifndef VNLC_OUT_OF_RANGE_ERROR_HPP
#define VNLC_OUT_OF_RANGE_ERROR_HPP

#include "VnlcError.hpp"
#include <fmt/core.h>

class VnlcOutOfRangeError : public VnlcError {
public:
    VnlcOutOfRangeError(const std::string& message) : VnlcError(fmt::format("Out of range during iteration: {}", message)) {}
};

#endif // VNLC_OUT_OF_RANGE_ERROR_HPP
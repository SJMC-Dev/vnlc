#ifndef VNLC_PACKAGE_READER_ERROR_HPP
#define VNLC_PACKAGE_READER_ERROR_HPP

#include "VnlcError.hpp"
#include <fmt/core.h>

class VnlcPackageReaderError : public VnlcError {
public:
    VnlcPackageReaderError(std::string_view message) : VnlcError(fmt::format("Error reading package: {}", message)) {}
};

#endif // VNLC_PACKAGE_READER_ERROR_HPP
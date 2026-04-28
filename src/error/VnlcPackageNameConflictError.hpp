#pragma once

#ifndef VNLC_PACKAGE_NAME_CONFLICT_ERROR_HPP
#define VNLC_PACKAGE_NAME_CONFLICT_ERROR_HPP

#include "VnlcError.hpp"

class VnlcPackageNameConflictError : public VnlcError {
public:
    VnlcPackageNameConflictError(std::string_view packageName) : VnlcError("Found two or more packages with the name: " + std::string(packageName)) {}
};

#endif // VNLC_PACKAGE_NAME_CONFLICT_ERROR_HPP
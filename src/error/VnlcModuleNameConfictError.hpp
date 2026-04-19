#pragma once

#ifndef VNLC_MODUULE_NAME_CONFLICT_ERROR_HPP
#define VNLC_MODUULE_NAME_CONFLICT_ERROR_HPP

#include "VnlcError.hpp"

class VnlcModuleNameConflictError : public VnlcError {
public:
    VnlcModuleNameConflictError(std::string_view moduleName) : VnlcError("Found two or more modules with the name: " + std::string(moduleName)) {}
};

#endif // VNLC_MODUULE_NAME_CONFLICT_ERROR_HPP
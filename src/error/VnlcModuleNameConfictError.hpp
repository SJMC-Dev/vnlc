#pragma once

#ifndef __VNLC_MODUULE_NAME_CONFLICT_ERROR_HPP__
#define __VNLC_MODUULE_NAME_CONFLICT_ERROR_HPP__

#include "VnlcError.hpp"

class VnlcModuleNameConflictError : public VnlcError {
public:
    VnlcModuleNameConflictError(std::string_view moduleName) : VnlcError("Found two or more modules with the name: " + std::string(moduleName)) {}
};

#endif // __VNLC_MODUULE_NAME_CONFLICT_ERROR_HPP__
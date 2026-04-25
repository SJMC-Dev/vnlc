#include "VnlcModule.hpp"

VnlcModule::VnlcModule(std::string_view fullName, VnlcToken&& firstToken, VnlcToken&& lastToken) noexcept
    : VnlcAstNode(std::move(firstToken), std::move(lastToken)),
      fullName(fullName),
      name(fullName.substr(fullName.find_last_of('.') + 1)) {}

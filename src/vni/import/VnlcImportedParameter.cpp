#include "VnlcImportedParameter.hpp"

VnlcImportedParameter::VnlcImportedParameter(std::string_view name, std::string_view type, bool readonly, std::unordered_map<std::string, std::optional<std::string>>&& metadata)
    : VnlcImportedIdentifier(name, std::move(metadata)),
      type(type),
      readonly(readonly) {}

VnlcImportedParameter::VnlcImportedParameter(std::string_view name, std::string_view type, bool readonly) : VnlcImportedIdentifier(name), type(type), readonly(readonly) {}

std::string_view VnlcImportedParameter::getType() const {
    return type;
}

bool VnlcImportedParameter::isReadonly() const {
    return readonly;
}

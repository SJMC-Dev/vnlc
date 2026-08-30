#include "VnlcImportedEnumValue.hpp"

VnlcImportedEnumValue::VnlcImportedEnumValue(std::string_view name, std::string_view type, std::unordered_map<std::string, std::optional<std::string>>&& metadata)
    : VnlcImportedIdentifier(name, std::move(metadata)),
      type(type) {}

VnlcImportedEnumValue::VnlcImportedEnumValue(std::string_view name, std::string_view type) : VnlcImportedIdentifier(name), type(type) {}

std::string_view VnlcImportedEnumValue::getType() const {
    return type;
}
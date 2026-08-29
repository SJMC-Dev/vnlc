#include "VnlcImportedProperty.hpp"

VnlcImportedProperty::VnlcImportedProperty(
    std::string_view name,
    std::string_view type,
    std::string_view accessModifier,
    std::unordered_map<std::string, std::optional<std::string>>&& metadata
)
    : VnlcImportedIdentifier(name, std::move(metadata)),
      type(type),
      accessModifier(accessModifier) {}

VnlcImportedProperty::VnlcImportedProperty(std::string_view name, std::string_view type, std::string_view accessModifier)
    : VnlcImportedIdentifier(name),
      type(type),
      accessModifier(accessModifier) {}

std::string_view VnlcImportedProperty::getType() const {
    return type;
}

std::string_view VnlcImportedProperty::getAccessModifier() const {
    return accessModifier;
}

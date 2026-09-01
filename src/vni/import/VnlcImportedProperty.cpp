#include "VnlcImportedProperty.hpp"

VnlcImportedProperty::VnlcImportedProperty(
    std::string_view name,
    std::string_view type,
    bool staticProperty,
    std::string_view accessModifier,
    std::unordered_map<std::string, std::optional<std::string>>&& metadata
)
    : VnlcImportedIdentifier(name, std::move(metadata)),
      type(type),
      staticProperty(staticProperty),
      accessModifier(accessModifier) {}

VnlcImportedProperty::VnlcImportedProperty(std::string_view name, std::string_view type, bool staticProperty, std::string_view accessModifier)
    : VnlcImportedIdentifier(name),
      type(type),
      staticProperty(staticProperty),
      accessModifier(accessModifier) {}

std::string_view VnlcImportedProperty::getType() const {
    return type;
}

bool VnlcImportedProperty::isStatic() const {
    return staticProperty;
}

std::string_view VnlcImportedProperty::getAccessModifier() const {
    return accessModifier;
}

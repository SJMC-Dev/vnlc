#include "VnlcImportedProperty.hpp"

VnlcImportedProperty::VnlcImportedProperty(
    std::string_view name,
    std::string_view type,
    bool readonly,
    std::string_view accessModifier,
    std::unordered_map<std::string, std::optional<std::string>>&& metadata
)
    : VnlcImportedIdentifier(name, std::move(metadata)),
      type(type),
      readonly(readonly),
      accessModifier(accessModifier) {}

VnlcImportedProperty::VnlcImportedProperty(std::string_view name, std::string_view type, bool readonly, std::string_view accessModifier)
    : VnlcImportedIdentifier(name),
      type(type),
      readonly(readonly),
      accessModifier(accessModifier) {}

std::string_view VnlcImportedProperty::getType() const {
    return type;
}

bool VnlcImportedProperty::isReadonly() const {
    return readonly;
}

std::string_view VnlcImportedProperty::getAccessModifier() const {
    return accessModifier;
}

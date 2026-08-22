#include "VnlcImportedModule.hpp"

VnlcImportedModule::VnlcImportedModule(std::string_view name, std::unordered_map<std::string, std::unique_ptr<VnlcImportedIdentifier>>&& identifiers)
    : VnlcImportedItem(name),
      identifiers(std::move(identifiers)) {}

const std::unordered_map<std::string, std::unique_ptr<VnlcImportedIdentifier>>& VnlcImportedModule::getIdentifiers() const {
    return identifiers;
}

std::optional<const VnlcImportedIdentifier*> VnlcImportedModule::getIdentifierByName(std::string_view name) const {
    auto it = identifiers.find(std::string(name));
    if (it != identifiers.end()) {
        return std::make_optional<const VnlcImportedIdentifier*>(it->second.get());
    }
    return std::nullopt;
}
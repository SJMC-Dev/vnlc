#include "VnlcImportedIdentifier.hpp"

VnlcImportedIdentifier::VnlcImportedIdentifier(std::string_view name, std::unordered_map<std::string, std::optional<std::string>>&& metadata)
    : VnlcImportedItem(name),
      metadata(std::move(metadata)) {}

VnlcImportedIdentifier::VnlcImportedIdentifier(std::string_view name) : VnlcImportedItem(name), metadata({}) {}

const std::unordered_map<std::string, std::optional<std::string>>& VnlcImportedIdentifier::getMetadata() const {
    return metadata;
}
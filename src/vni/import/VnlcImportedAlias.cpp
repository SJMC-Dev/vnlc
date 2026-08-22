#include "VnlcImportedAlias.hpp"

VnlcImportedAlias::VnlcImportedAlias(std::string_view name, std::string_view source, std::unordered_map<std::string, std::optional<std::string>>&& metadata)
    : VnlcImportedIdentifier(name, std::move(metadata)),
      source(source) {}

VnlcImportedAlias::VnlcImportedAlias(std::string_view name, std::string_view source) : VnlcImportedIdentifier(name), source(source) {}

std::string_view VnlcImportedAlias::getSource() const {
    return source;
}

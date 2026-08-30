#include "VnlcImportedAlias.hpp"

VnlcImportedAlias::VnlcImportedAlias(std::string_view name, std::string_view source) : VnlcImportedIdentifier(name), source(source) {}

std::string_view VnlcImportedAlias::getSource() const {
    return source;
}

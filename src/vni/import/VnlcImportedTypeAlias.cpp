#include "VnlcImportedTypeAlias.hpp"

VnlcImportedTypeAlias::VnlcImportedTypeAlias(
    std::string_view name,
    std::vector<std::string>&& genericParameters,
    std::string_view originalType,
    std::unordered_map<std::string, std::optional<std::string>>&& metadata
)
    : VnlcImportedIdentifier(name, std::move(metadata)),
      genericParameters(std::move(genericParameters)),
      originalType(originalType) {}

VnlcImportedTypeAlias::VnlcImportedTypeAlias(std::string_view name, std::vector<std::string>&& genericParameters, std::string_view originalType)
    : VnlcImportedIdentifier(name),
      genericParameters(std::move(genericParameters)),
      originalType(originalType) {}

const std::vector<std::string>& VnlcImportedTypeAlias::getGenericParameters() const {
    return genericParameters;
}

std::string_view VnlcImportedTypeAlias::getOriginalType() const {
    return originalType;
}

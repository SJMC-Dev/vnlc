#include "VnlcImportedEnumMember.hpp"

VnlcImportedEnumMember::VnlcImportedEnumMember(
    std::string_view name,
    std::unordered_map<std::string, std::unique_ptr<VnlcImportedEnumValue>>&& associatedValues,
    std::unordered_map<std::string, std::optional<std::string>>&& metadata
)
    : VnlcImportedIdentifier(name, std::move(metadata)),
      associatedValues(std::move(associatedValues)) {}

VnlcImportedEnumMember::VnlcImportedEnumMember(std::string_view name, std::unordered_map<std::string, std::unique_ptr<VnlcImportedEnumValue>>&& associatedValues)
    : VnlcImportedIdentifier(name),
      associatedValues(std::move(associatedValues)) {}

const std::unordered_map<std::string, std::unique_ptr<VnlcImportedEnumValue>>& VnlcImportedEnumMember::getAssociatedValues() const {
    return associatedValues;
}
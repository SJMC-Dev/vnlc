#include "VnlcImportedClass.hpp"

VnlcImportedClass::VnlcImportedClass(
    std::string_view name,
    bool final,
    std::vector<std::string>&& genericParameters,
    std::unordered_map<std::string, std::unique_ptr<VnlcImportedProperty>>&& properties,
    std::unordered_map<std::string, std::unique_ptr<VnlcImportedMethod>>&& methods,
    std::unordered_map<std::string, std::optional<std::string>>&& metadata
)
    : VnlcImportedIdentifier(name, std::move(metadata)),
      final(final),
      genericParameters(std::move(genericParameters)),
      properties(std::move(properties)),
      methods(std::move(methods)) {}

VnlcImportedClass::VnlcImportedClass(
    std::string_view name,
    bool final,
    std::vector<std::string>&& genericParameters,
    std::unordered_map<std::string, std::unique_ptr<VnlcImportedProperty>>&& properties,
    std::unordered_map<std::string, std::unique_ptr<VnlcImportedMethod>>&& methods
)
    : VnlcImportedIdentifier(name),
      final(final),
      genericParameters(std::move(genericParameters)),
      properties(std::move(properties)),
      methods(std::move(methods)) {}

bool VnlcImportedClass::isFinal() const {
    return final;
}

const std::vector<std::string>& VnlcImportedClass::getGenericParameters() const {
    return genericParameters;
}

const std::unordered_map<std::string, std::unique_ptr<VnlcImportedProperty>>& VnlcImportedClass::getProperties() const {
    return properties;
}

const std::unordered_map<std::string, std::unique_ptr<VnlcImportedMethod>>& VnlcImportedClass::getMethods() const {
    return methods;
}

std::optional<const VnlcImportedProperty*> VnlcImportedClass::getPropertyByName(std::string_view name) const {
    auto it = properties.find(std::string(name));
    if (it != properties.end()) {
        return std::make_optional<const VnlcImportedProperty*>(it->second.get());
    }
    return std::nullopt;
}

std::optional<const VnlcImportedMethod*> VnlcImportedClass::getMethodByName(std::string_view name) const {
    auto it = methods.find(std::string(name));
    if (it != methods.end()) {
        return std::make_optional<const VnlcImportedMethod*>(it->second.get());
    }
    return std::nullopt;
}

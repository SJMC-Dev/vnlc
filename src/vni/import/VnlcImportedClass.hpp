#ifndef VNLC_IMPORTED_CLASS_HPP
#define VNLC_IMPORTED_CLASS_HPP

#include "VnlcImportedIdentifier.hpp"
#include "VnlcImportedMethod.hpp"
#include "VnlcImportedProperty.hpp"
#include <memory>
#include <unordered_map>
#include <vector>

class VnlcImportedClass : public VnlcImportedIdentifier {
private:
    bool final;
    std::vector<std::string> genericParameters;
    std::unordered_map<std::string, std::unique_ptr<VnlcImportedProperty>> properties;
    std::unordered_map<std::string, std::unique_ptr<VnlcImportedMethod>> methods;

public:
    VnlcImportedClass(
        std::string_view name,
        bool final,
        std::vector<std::string>&& genericParameters,
        std::unordered_map<std::string, std::unique_ptr<VnlcImportedProperty>>&& properties,
        std::unordered_map<std::string, std::unique_ptr<VnlcImportedMethod>>&& methods,
        std::unordered_map<std::string, std::optional<std::string>>&& metadata
    );

    VnlcImportedClass(
        std::string_view name,
        bool final,
        std::vector<std::string>&& genericParameters,
        std::unordered_map<std::string, std::unique_ptr<VnlcImportedProperty>>&& properties,
        std::unordered_map<std::string, std::unique_ptr<VnlcImportedMethod>>&& methods
    );

    [[nodiscard]] bool isFinal() const;
    [[nodiscard]] const std::vector<std::string>& getGenericParameters() const;
    [[nodiscard]] const std::unordered_map<std::string, std::unique_ptr<VnlcImportedProperty>>& getProperties() const;
    [[nodiscard]] const std::unordered_map<std::string, std::unique_ptr<VnlcImportedMethod>>& getMethods() const;

    [[nodiscard]] std::optional<const VnlcImportedProperty*> getPropertyByName(std::string_view name) const;
    [[nodiscard]] std::optional<const VnlcImportedMethod*> getMethodByName(std::string_view name) const;
};

#endif // VNLC_IMPORTED_CLASS_HPP

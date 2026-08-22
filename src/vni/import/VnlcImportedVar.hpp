#ifndef VNLC_IMPORTED_VAR_HPP
#define VNLC_IMPORTED_VAR_HPP

#include "VnlcImportedIdentifier.hpp"

class VnlcImportedVar : public VnlcImportedIdentifier {
private:
    std::string type;
    bool readonly;

public:
    VnlcImportedVar(std::string_view name, std::string_view type, bool readonly, std::unordered_map<std::string, std::optional<std::string>>&& metadata);
    VnlcImportedVar(std::string_view name, std::string_view type, bool readonly);

    [[nodiscard]] std::string_view getType() const;
    [[nodiscard]] bool isReadonly() const;
};

#endif // VNLC_IMPORTED_VAR_HPP
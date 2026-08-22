#ifndef VNLC_IMPORTED_CONST_HPP
#define VNLC_IMPORTED_CONST_HPP

#include "VnlcImportedIdentifier.hpp"
#include <nlohmann/json.hpp>

class VnlcImportedConst : public VnlcImportedIdentifier {
private:
    std::string type;
    nlohmann::json value;

public:
    VnlcImportedConst(std::string_view name, std::string_view type, nlohmann::json&& value, std::unordered_map<std::string, std::optional<std::string>>&& metadata);
    VnlcImportedConst(std::string_view name, std::string_view type, nlohmann::json&& value);

    [[nodiscard]] std::string_view getType() const;
    [[nodiscard]] const nlohmann::json& getValue() const;
};

#endif // VNLC_IMPORTED_CONST_HPP

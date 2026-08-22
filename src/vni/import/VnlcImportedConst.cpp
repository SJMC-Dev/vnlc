#include "VnlcImportedConst.hpp"

VnlcImportedConst::VnlcImportedConst(std::string_view name, std::string_view type, nlohmann::json&& value, std::unordered_map<std::string, std::optional<std::string>>&& metadata)
    : VnlcImportedIdentifier(name, std::move(metadata)),
      type(type),
      value(std::move(value)) {}

VnlcImportedConst::VnlcImportedConst(std::string_view name, std::string_view type, nlohmann::json&& value) : VnlcImportedIdentifier(name), type(type), value(std::move(value)) {}

std::string_view VnlcImportedConst::getType() const {
    return type;
}

const nlohmann::json& VnlcImportedConst::getValue() const {
    return value;
}

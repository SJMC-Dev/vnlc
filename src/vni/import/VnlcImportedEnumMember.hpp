#ifndef VNLC_IMPORTED_ENUM_MEMBER_HPP
#define VNLC_IMPORTED_ENUM_MEMBER_HPP

#include "VnlcImportedIdentifier.hpp"

class VnlcImportedEnumMember : public VnlcImportedIdentifier {
private:
    std::string type;
    bool readonly;

public:
    VnlcImportedEnumMember(std::string_view name, std::string_view type, bool readonly, std::unordered_map<std::string, std::optional<std::string>>&& metadata);
    VnlcImportedEnumMember(std::string_view name, std::string_view type, bool readonly);

    [[nodiscard]] std::string_view getType() const;
    [[nodiscard]] bool isReadonly() const;
};

#endif // VNLC_IMPORTED_ENUM_MEMBER_HPP

#ifndef VNLC_IMPORTED_ENUM_MEMBER_HPP
#define VNLC_IMPORTED_ENUM_MEMBER_HPP

#include "VnlcImportedIdentifier.hpp"

class VnlcImportedEnumMember : public VnlcImportedIdentifier {
private:
    std::string type;

public:
    VnlcImportedEnumMember(std::string_view name, std::string_view type, std::unordered_map<std::string, std::optional<std::string>>&& metadata);
    VnlcImportedEnumMember(std::string_view name, std::string_view type);

    [[nodiscard]] std::string_view getType() const;
};

#endif // VNLC_IMPORTED_ENUM_MEMBER_HPP

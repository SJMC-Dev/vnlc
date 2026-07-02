#ifndef VNLC_DECLARATION_ITEM_HPP
#define VNLC_DECLARATION_ITEM_HPP

#include <optional>
#include <string>

namespace VnlcDeclarationItem {
    struct MetadataTerm {
        std::string key;
        std::optional<std::string> value; // if value is not provided, it is considered as "true"
    };
} // namespace VnlcDeclarationItem

#endif // VNLC_DECLARATION_ITEM_HPP
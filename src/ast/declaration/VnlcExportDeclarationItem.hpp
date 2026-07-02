#ifndef VNLC_EXPORT_DECLARATION_ITEM_HPP
#define VNLC_EXPORT_DECLARATION_ITEM_HPP

#include <optional>
#include <string>

struct VnlcExportDeclarationItem {
    std::string name;
    std::optional<std::string> alias; // nullopt if no alias
};

#endif // VNLC_EXPORT_DECLARATION_ITEM_HPP
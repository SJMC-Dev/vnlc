#ifndef VNLC_IMPORT_DECLARATION_ITEM_HPP
#define VNLC_IMPORT_DECLARATION_ITEM_HPP

#include <optional>
#include <string>
#include <vector>

struct VnlcImportDeclarationItem {
    std::vector<std::string> nameParts;
    std::optional<std::string> alias; // nullopt if no alias or wildcard import
};

#endif // VNLC_IMPORT_DECLARATION_ITEM_HPP
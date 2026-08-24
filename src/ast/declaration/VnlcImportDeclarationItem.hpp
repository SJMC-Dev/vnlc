#ifndef VNLC_IMPORT_DECLARATION_ITEM_HPP
#define VNLC_IMPORT_DECLARATION_ITEM_HPP

#include "../identifier/VnlcIdentifierNode.hpp"
#include <memory>
#include <optional>
#include <vector>

struct VnlcImportDeclarationItem {
    std::vector<std::unique_ptr<VnlcIdentifierNode>> namePrefix;
    std::optional<std::vector<std::unique_ptr<VnlcImportDeclarationItem>>> nameSuffices;
    std::optional<std::unique_ptr<VnlcIdentifierNode>> alias;
    bool self = false;
    bool wildcard = false;
};

#endif // VNLC_IMPORT_DECLARATION_ITEM_HPP
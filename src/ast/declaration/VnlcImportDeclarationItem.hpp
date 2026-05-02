#pragma once

#ifndef VNLC_IMPORT_DECLARATION_ITEM_HPP
#define VNLC_IMPORT_DECLARATION_ITEM_HPP

#include <optional>
#include <string>
#include <vector>

struct VnlcImportDeclarationItem {
    std::vector<std::string> nameParts;
    std::optional<std::string> alias; // nullopt if no alias
};

#endif // VNLC_IMPORT_DECLARATION_ITEM_HPP
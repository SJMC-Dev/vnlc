#pragma once

#ifndef VNLC_ABSOLUTE_IMPORT_PATH_ITEM_PARSING_RESULT_HPP
#define VNLC_ABSOLUTE_IMPORT_PATH_ITEM_PARSING_RESULT_HPP

#include "../../ast/declaration/VnlcImportDeclarationItem.hpp"
#include <vector>

struct VnlcAbsoluteImportPathItemParsingResult {
    std::vector<VnlcImportDeclarationItem> items;
};

#endif // VNLC_ABSOLUTE_IMPORT_PATH_ITEM_PARSING_RESULT_HPP

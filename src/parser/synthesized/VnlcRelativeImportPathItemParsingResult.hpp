#pragma once

#ifndef VNLC_RELATIVE_IMPORT_PATH_ITEM_PARSING_RESULT_HPP
#define VNLC_RELATIVE_IMPORT_PATH_ITEM_PARSING_RESULT_HPP

#include "../../ast/declaration/VnlcImportDeclarationItem.hpp"
#include <vector>

struct VnlcRelativeImportPathItemParsingResult {
    std::vector<VnlcImportDeclarationItem> paths;
};

#endif // VNLC_RELATIVE_IMPORT_PATH_ITEM_PARSING_RESULT_HPP

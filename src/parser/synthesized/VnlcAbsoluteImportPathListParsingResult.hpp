#ifndef VNLC_ABSOLUTE_IMPORT_PATH_LIST_PARSING_RESULT_HPP
#define VNLC_ABSOLUTE_IMPORT_PATH_LIST_PARSING_RESULT_HPP

#include "../../ast/declaration/VnlcImportDeclarationItem.hpp"
#include <vector>

struct VnlcAbsoluteImportPathListParsingResult {
    std::vector<VnlcImportDeclarationItem> paths;
};

#endif // VNLC_ABSOLUTE_IMPORT_PATH_LIST_PARSING_RESULT_HPP

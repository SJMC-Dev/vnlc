#pragma once

#ifndef VNLC_IMPORT_PATH_PARSING_RESULT_HPP
#define VNLC_IMPORT_PATH_PARSING_RESULT_HPP

#include "../../ast/declaration/VnlcImportDeclarationItem.hpp"
#include <vector>

struct VnlcImportPathParsingResult {
    bool relative;
    std::vector<VnlcImportDeclarationItem> paths;
};

#endif // VNLC_IMPORT_PATH_PARSING_RESULT_HPP

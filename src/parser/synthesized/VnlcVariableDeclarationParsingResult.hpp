#pragma once

#ifndef VNLC_VARIABLE_DECLARATION_PARSING_RESULT_HPP
#define VNLC_VARIABLE_DECLARATION_PARSING_RESULT_HPP

#include "../../ast/declaration/VnlcVariableDeclarationNode.hpp"
#include <memory>

struct VnlcVariableDeclarationParsingResult {
    std::unique_ptr<VnlcVariableDeclarationNode> declaration;
};

#endif // VNLC_VARIABLE_DECLARATION_PARSING_RESULT_HPP

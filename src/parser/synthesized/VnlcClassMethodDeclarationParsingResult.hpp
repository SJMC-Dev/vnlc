#pragma once

#ifndef VNLC_CLASS_METHOD_DECLARATION_PARSING_RESULT_HPP
#define VNLC_CLASS_METHOD_DECLARATION_PARSING_RESULT_HPP

#include "../../ast/declaration/VnlcFunctionDeclarationNode.hpp"
#include <memory>

struct VnlcClassMethodDeclarationParsingResult {
    std::unique_ptr<VnlcFunctionDeclarationNode> declaration;
};

#endif // VNLC_CLASS_METHOD_DECLARATION_PARSING_RESULT_HPP

#pragma once

#ifndef VNLC_LIST_LITERAL_PARSING_RESULT_HPP
#define VNLC_LIST_LITERAL_PARSING_RESULT_HPP

#include "../../ast/expression/VnlcExpressionNode.hpp"
#include <memory>

struct VnlcListLiteralParsingResult {
    std::unique_ptr<VnlcExpressionNode> expression;
};

#endif // VNLC_LIST_LITERAL_PARSING_RESULT_HPP

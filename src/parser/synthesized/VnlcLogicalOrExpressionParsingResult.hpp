#pragma once

#ifndef VNLC_LOGICAL_OR_EXPRESSION_PARSING_RESULT_HPP
#define VNLC_LOGICAL_OR_EXPRESSION_PARSING_RESULT_HPP

#include "../../ast/expression/VnlcBinaryExpressionNode.hpp"
#include <memory>

struct VnlcLogicalOrExpressionParsingResult {
    std::unique_ptr<VnlcBinaryExpressionNode> expression;
};

#endif // VNLC_LOGICAL_OR_EXPRESSION_PARSING_RESULT_HPP

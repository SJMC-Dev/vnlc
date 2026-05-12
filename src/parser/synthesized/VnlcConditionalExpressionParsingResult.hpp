#pragma once

#ifndef VNLC_CONDITIONAL_EXPRESSION_PARSING_RESULT_HPP
#define VNLC_CONDITIONAL_EXPRESSION_PARSING_RESULT_HPP

#include "../../ast/expression/VnlcConditionalExpressionNode.hpp"
#include <memory>

struct VnlcConditionalExpressionParsingResult {
    std::unique_ptr<VnlcConditionalExpressionNode> expression;
};

#endif // VNLC_CONDITIONAL_EXPRESSION_PARSING_RESULT_HPP

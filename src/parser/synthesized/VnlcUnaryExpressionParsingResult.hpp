#pragma once

#ifndef VNLC_UNARY_EXPRESSION_PARSING_RESULT_HPP
#define VNLC_UNARY_EXPRESSION_PARSING_RESULT_HPP

#include "../../ast/expression/VnlcUnaryExpressionNode.hpp"
#include <memory>

struct VnlcUnaryExpressionParsingResult {
    std::unique_ptr<VnlcUnaryExpressionNode> expression;
};

#endif // VNLC_UNARY_EXPRESSION_PARSING_RESULT_HPP

#pragma once

#ifndef VNLC_MULTIPLICATIVE_EXPRESSION_PARSING_RESULT_HPP
#define VNLC_MULTIPLICATIVE_EXPRESSION_PARSING_RESULT_HPP

#include "../../ast/expression/VnlcBinaryExpressionNode.hpp"
#include <memory>

struct VnlcMultiplicativeExpressionParsingResult {
    std::unique_ptr<VnlcBinaryExpressionNode> expression;
};

#endif // VNLC_MULTIPLICATIVE_EXPRESSION_PARSING_RESULT_HPP

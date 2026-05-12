#pragma once

#ifndef VNLC_BITWISE_OR_EXPRESSION_PARSING_RESULT_HPP
#define VNLC_BITWISE_OR_EXPRESSION_PARSING_RESULT_HPP

#include "../../ast/expression/VnlcBinaryExpressionNode.hpp"
#include <memory>

struct VnlcBitwiseOrExpressionParsingResult {
    std::unique_ptr<VnlcBinaryExpressionNode> expression;
};

#endif // VNLC_BITWISE_OR_EXPRESSION_PARSING_RESULT_HPP

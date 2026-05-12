#pragma once

#ifndef VNLC_BITWISE_AND_EXPRESSION_PARSING_RESULT_HPP
#define VNLC_BITWISE_AND_EXPRESSION_PARSING_RESULT_HPP

#include "../../ast/expression/VnlcBinaryExpressionNode.hpp"
#include <memory>

struct VnlcBitwiseAndExpressionParsingResult {
    std::unique_ptr<VnlcBinaryExpressionNode> expression;
};

#endif // VNLC_BITWISE_AND_EXPRESSION_PARSING_RESULT_HPP

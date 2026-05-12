#pragma once

#ifndef VNLC_ADDITIVE_EXPRESSION_PARSING_RESULT_HPP
#define VNLC_ADDITIVE_EXPRESSION_PARSING_RESULT_HPP

#include "../../ast/expression/VnlcBinaryExpressionNode.hpp"
#include <memory>

struct VnlcAdditiveExpressionParsingResult {
    std::unique_ptr<VnlcBinaryExpressionNode> expression;
};

#endif // VNLC_ADDITIVE_EXPRESSION_PARSING_RESULT_HPP

#pragma once

#ifndef VNLC_LOGICAL_AND_EXPRESSION_PARSING_RESULT_HPP
#define VNLC_LOGICAL_AND_EXPRESSION_PARSING_RESULT_HPP

#include "../../ast/expression/VnlcBinaryExpressionNode.hpp"
#include <memory>

struct VnlcLogicalAndExpressionParsingResult {
    std::unique_ptr<VnlcBinaryExpressionNode> expression;
};

#endif // VNLC_LOGICAL_AND_EXPRESSION_PARSING_RESULT_HPP

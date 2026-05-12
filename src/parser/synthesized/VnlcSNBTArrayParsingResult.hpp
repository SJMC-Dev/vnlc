#pragma once

#ifndef VNLC_SNBT_ARRAY_PARSING_RESULT_HPP
#define VNLC_SNBT_ARRAY_PARSING_RESULT_HPP

#include "../../ast/expression/VnlcListLikeLiteralExpressionNode.hpp"
#include <memory>

struct VnlcSNBTArrayParsingResult {
    std::unique_ptr<VnlcListLikeLiteralExpressionNode> expression;
};

#endif // VNLC_SNBT_ARRAY_PARSING_RESULT_HPP

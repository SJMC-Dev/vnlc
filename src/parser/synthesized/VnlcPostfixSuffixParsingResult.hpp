#pragma once

#ifndef VNLC_POSTFIX_SUFFIX_PARSING_RESULT_HPP
#define VNLC_POSTFIX_SUFFIX_PARSING_RESULT_HPP

#include "../../ast/expression/VnlcExpressionNode.hpp"
#include <memory>

struct VnlcPostfixSuffixParsingResult {
    std::unique_ptr<VnlcExpressionNode> expression;
};

#endif // VNLC_POSTFIX_SUFFIX_PARSING_RESULT_HPP

#pragma once

#ifndef VNLC_STRING_PARSING_RESULT_HPP
#define VNLC_STRING_PARSING_RESULT_HPP

#include "../../ast/expression/VnlcStringLiteralExpressionNode.hpp"
#include <memory>

struct VnlcStringParsingResult {
    std::unique_ptr<VnlcStringLiteralExpressionNode> expression;
};

#endif // VNLC_STRING_PARSING_RESULT_HPP

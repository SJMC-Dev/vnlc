#pragma once

#ifndef VNLC_LITERAL_PARSING_RESULT_HPP
#define VNLC_LITERAL_PARSING_RESULT_HPP

#include "../../ast/expression/VnlcLiteralExpressionNode.hpp"
#include <memory>

struct VnlcLiteralParsingResult {
    std::unique_ptr<VnlcLiteralExpressionNode> expression;
};

#endif // VNLC_LITERAL_PARSING_RESULT_HPP

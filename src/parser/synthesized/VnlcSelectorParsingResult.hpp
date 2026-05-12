#pragma once

#ifndef VNLC_SELECTOR_PARSING_RESULT_HPP
#define VNLC_SELECTOR_PARSING_RESULT_HPP

#include "../../ast/expression/VnlcSelectorLiteralExpressionNode.hpp"
#include <memory>

struct VnlcSelectorParsingResult {
    std::unique_ptr<VnlcSelectorLiteralExpressionNode> expression;
};

#endif // VNLC_SELECTOR_PARSING_RESULT_HPP

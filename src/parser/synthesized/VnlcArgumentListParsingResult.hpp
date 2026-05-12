#pragma once

#ifndef VNLC_ARGUMENT_LIST_PARSING_RESULT_HPP
#define VNLC_ARGUMENT_LIST_PARSING_RESULT_HPP

#include "../../ast/expression/VnlcExpressionNode.hpp"
#include <memory>
#include <vector>

struct VnlcArgumentListParsingResult {
    std::vector<std::unique_ptr<VnlcExpressionNode>> arguments;
};

#endif // VNLC_ARGUMENT_LIST_PARSING_RESULT_HPP

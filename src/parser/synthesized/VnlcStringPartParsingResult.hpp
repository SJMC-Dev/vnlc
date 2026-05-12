#pragma once

#ifndef VNLC_STRING_PART_PARSING_RESULT_HPP
#define VNLC_STRING_PART_PARSING_RESULT_HPP

#include "../../ast/expression/VnlcExpressionNode.hpp"
#include <memory>
#include <string>
#include <variant>

struct VnlcStringPartParsingResult {
    std::variant<std::string, std::unique_ptr<VnlcExpressionNode>> part;
};

#endif // VNLC_STRING_PART_PARSING_RESULT_HPP

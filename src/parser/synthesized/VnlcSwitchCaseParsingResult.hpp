#pragma once

#ifndef VNLC_SWITCH_CASE_PARSING_RESULT_HPP
#define VNLC_SWITCH_CASE_PARSING_RESULT_HPP

#include "../../ast/expression/VnlcLiteralExpressionNode.hpp"
#include "../../ast/other/VnlcTypeNode.hpp"
#include "../../ast/statement/VnlcSwitchStatementType.hpp"
#include <optional>

struct VnlcSwitchCaseParsingResult {
    VnlcSwitchStatementType kind;
    std::optional<std::unique_ptr<VnlcLiteralExpressionNode>> literal;
    std::optional<std::unique_ptr<VnlcTypeNode>> type;
};

#endif // VNLC_SWITCH_CASE_PARSING_RESULT_HPP

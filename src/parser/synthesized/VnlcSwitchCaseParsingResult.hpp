#pragma once

#ifndef VNLC_SWITCH_CASE_PARSING_RESULT_HPP
#define VNLC_SWITCH_CASE_PARSING_RESULT_HPP

#include "../../ast/statement/VnlcSwitchStatementItem.hpp"
#include "../../ast/statement/VnlcSwitchStatementType.hpp"
#include <optional>

struct VnlcSwitchCaseParsingResult {
    VnlcSwitchStatementType kind;
    std::optional<VnlcSwitchStatementItem::LiteralMatchItem> literalMatchItem;
    std::optional<VnlcSwitchStatementItem::TypeMatchItem> typeMatchItem;
};

#endif // VNLC_SWITCH_CASE_PARSING_RESULT_HPP

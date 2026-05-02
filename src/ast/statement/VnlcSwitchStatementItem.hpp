#pragma once

#ifndef VNLC_SWITCH_STATEMENT_ITEM_HPP
#define VNLC_SWITCH_STATEMENT_ITEM_HPP

#include "../expression/VnlcExpressionNode.hpp"
#include "../expression/VnlcLiteralExpressionNode.hpp"
#include "VnlcStatementNode.hpp"
#include <memory>
#include <optional>
#include <vector>

namespace VnlcSwitchStatementItem {
    struct LiteralMatchItem {
        std::unique_ptr<VnlcLiteralExpressionNode> literal;
        std::unique_ptr<VnlcStatementNode> body;
    };

    struct TypeMatchItem {
        std::vector<std::string> typeNameParts;
        std::unique_ptr<VnlcStatementNode> body;
        std::optional<std::unique_ptr<VnlcExpressionNode>> guardExpression;
    };
} // namespace VnlcSwitchStatementItem

#endif // VNLC_SWITCH_STATEMENT_ITEM_HPP
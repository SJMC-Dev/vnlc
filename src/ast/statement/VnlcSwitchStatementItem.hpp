#ifndef VNLC_SWITCH_STATEMENT_ITEM_HPP
#define VNLC_SWITCH_STATEMENT_ITEM_HPP

#include "../expression/VnlcExpressionNode.hpp"
#include "../type/VnlcTypeNode.hpp"
#include "VnlcStatementNode.hpp"
#include <memory>
#include <optional>

namespace VnlcSwitchStatementItem {
    struct LiteralMatchItem {
        std::unique_ptr<VnlcExpressionNode> literal;
        std::unique_ptr<VnlcStatementNode> body;
    };

    struct TypeMatchItem {
        std::unique_ptr<VnlcTypeNode> type;
        std::unique_ptr<VnlcStatementNode> body;
        std::optional<std::unique_ptr<VnlcExpressionNode>> guardExpression;
    };
} // namespace VnlcSwitchStatementItem

#endif // VNLC_SWITCH_STATEMENT_ITEM_HPP
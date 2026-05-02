#include "VnlcSwitchStatementNode.hpp"

VnlcSwitchStatementNode::VnlcSwitchStatementNode(
    std::unique_ptr<VnlcExpressionNode>&& switchExpression,
    std::vector<VnlcSwitchStatementItem::LiteralMatchItem>&& literalMatchItems,
    std::unique_ptr<VnlcStatementNode>&& defaultCaseBody,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcControlFlowStatementNode(firstToken, lastToken),
      switchExpression(std::move(switchExpression)),
      switchType(VnlcSwitchStatementType::LITERAL_MATCH),
      literalMatchItems(std::move(literalMatchItems)),
      typeMatchItems(),
      defaultCaseBody(std::move(defaultCaseBody)) {}

VnlcSwitchStatementNode::VnlcSwitchStatementNode(
    std::unique_ptr<VnlcExpressionNode>&& switchExpression,
    std::vector<VnlcSwitchStatementItem::TypeMatchItem>&& typeMatchItems,
    std::unique_ptr<VnlcStatementNode>&& defaultCaseBody,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcControlFlowStatementNode(firstToken, lastToken),
      switchExpression(std::move(switchExpression)),
      switchType(VnlcSwitchStatementType::TYPE_MATCH),
      literalMatchItems(),
      typeMatchItems(std::move(typeMatchItems)),
      defaultCaseBody(std::move(defaultCaseBody)) {}

const VnlcExpressionNode& VnlcSwitchStatementNode::getSwitchExpression() const noexcept {
    return *switchExpression;
}

const VnlcSwitchStatementType VnlcSwitchStatementNode::getSwitchType() const noexcept {
    return switchType;
}

const std::vector<VnlcSwitchStatementItem::LiteralMatchItem>& VnlcSwitchStatementNode::getLiteralMatchItems() const noexcept {
    return literalMatchItems;
}

const std::vector<VnlcSwitchStatementItem::TypeMatchItem>& VnlcSwitchStatementNode::getTypeMatchItems() const noexcept {
    return typeMatchItems;
}

const VnlcStatementNode& VnlcSwitchStatementNode::getDefaultCaseBody() const noexcept {
    return *defaultCaseBody;
}
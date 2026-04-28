#include "VnlcRangeExpressionNode.hpp"

VnlcRangeExpressionNode::VnlcRangeExpressionNode(
    std::optional<std::unique_ptr<VnlcExpressionNode>>&& start,
    std::optional<std::unique_ptr<VnlcExpressionNode>>&& end,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcExpressionNode(firstToken, lastToken),
      start(std::move(start)),
      end(std::move(end)) {}

const std::optional<std::unique_ptr<VnlcExpressionNode>>& VnlcRangeExpressionNode::getStart() const noexcept {
    return start;
}

const std::optional<std::unique_ptr<VnlcExpressionNode>>& VnlcRangeExpressionNode::getEnd() const noexcept {
    return end;
}
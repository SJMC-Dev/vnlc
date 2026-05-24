#include "VnlcForStatementNode.hpp"

VnlcForStatementNode::VnlcForStatementNode(
    VnlcVariableDeclarationType loopVariableKind,
    std::string&& loopVariableName,
    std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>&& loopVariableTypeAnnotation,
    std::unique_ptr<VnlcExpressionNode>&& iterableExpression,
    std::unique_ptr<VnlcStatementNode>&& body,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcLoopStatementNode(std::nullopt, firstToken, lastToken),
      loopVariableKind(loopVariableKind),
      loopVariableName(std::move(loopVariableName)),
      loopVariableTypeAnnotation(std::move(loopVariableTypeAnnotation)),
      iterableExpression(std::move(iterableExpression)),
      body(std::move(body)) {}

VnlcForStatementNode::VnlcForStatementNode(
    std::string&& label,
    VnlcVariableDeclarationType loopVariableKind,
    std::string&& loopVariableName,
    std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>&& loopVariableTypeAnnotation,
    std::unique_ptr<VnlcExpressionNode>&& iterableExpression,
    std::unique_ptr<VnlcStatementNode>&& body,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcLoopStatementNode(std::make_optional(std::move(label)), firstToken, lastToken),
      loopVariableKind(loopVariableKind),
      loopVariableName(std::move(loopVariableName)),
      loopVariableTypeAnnotation(std::move(loopVariableTypeAnnotation)),
      iterableExpression(std::move(iterableExpression)),
      body(std::move(body)) {}

const VnlcVariableDeclarationType VnlcForStatementNode::getLoopVariableKind() const noexcept {
    return loopVariableKind;
}

std::string_view VnlcForStatementNode::getLoopVariableName() const noexcept {
    return loopVariableName;
}

const VnlcExpressionNode& VnlcForStatementNode::getIterableExpression() const noexcept {
    return *iterableExpression;
}

const VnlcStatementNode& VnlcForStatementNode::getBody() const noexcept {
    return *body;
}
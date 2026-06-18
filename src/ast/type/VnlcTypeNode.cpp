#include "VnlcTypeNode.hpp"

VnlcTypeNode::VnlcTypeNode(
    bool questionMarkSuffix,
    std::vector<std::string>&& nameParts,
    std::vector<std::unique_ptr<VnlcTypeNode>>&& genericArguments,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcAstNode(firstToken, lastToken),
      questionMarkSuffix(questionMarkSuffix),
      nameParts(std::move(nameParts)),
      genericArguments(std::move(genericArguments)) {}

const bool VnlcTypeNode::hasQuestionMarkSuffix() const noexcept {
    return questionMarkSuffix;
}

const std::vector<std::string>& VnlcTypeNode::getNameParts() const noexcept {
    return nameParts;
}

const std::vector<std::unique_ptr<VnlcTypeNode>>& VnlcTypeNode::getGenericArguments() const noexcept {
    return genericArguments;
}
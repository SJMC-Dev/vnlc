#include "VnlcEnumMemberDeclarationNode.hpp"

VnlcEnumMemberDeclarationNode::VnlcEnumMemberDeclarationNode(
    std::unique_ptr<VnlcIdentifierNode>&& name,
    std::vector<std::unique_ptr<VnlcValueDeclarationNode>>&& associatedValues,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken),
      name(std::move(name)),
      associatedValues(std::move(associatedValues)) {}

VnlcEnumMemberDeclarationNode::VnlcEnumMemberDeclarationNode(
    std::unique_ptr<VnlcIdentifierNode>&& name,
    std::vector<std::unique_ptr<VnlcValueDeclarationNode>>&& associatedValues,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken,
    std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken, std::move(metadataTerms)),
      name(std::move(name)),
      associatedValues(std::move(associatedValues)) {}

const VnlcIdentifierNode& VnlcEnumMemberDeclarationNode::getName() const noexcept {
    return *name;
}

const std::vector<std::unique_ptr<VnlcValueDeclarationNode>>& VnlcEnumMemberDeclarationNode::getAssociatedValues() const noexcept {
    return associatedValues;
}
#include "VnlcEnumMemberDeclarationNode.hpp"

VnlcEnumMemberDeclarationNode::VnlcEnumMemberDeclarationNode(
    std::string&& name,
    std::vector<std::unique_ptr<VnlcValueDeclarationNode>>&& associatedValues,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken),
      name(std::move(name)),
      associatedValues(std::move(associatedValues)) {}

VnlcEnumMemberDeclarationNode::VnlcEnumMemberDeclarationNode(
    std::string&& name,
    std::vector<std::unique_ptr<VnlcValueDeclarationNode>>&& associatedValues,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken,
    std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken, std::move(metadataTerms)),
      name(std::move(name)),
      associatedValues(std::move(associatedValues)) {}

std::string_view VnlcEnumMemberDeclarationNode::getName() const noexcept {
    return name;
}

const std::vector<std::unique_ptr<VnlcValueDeclarationNode>>& VnlcEnumMemberDeclarationNode::getAssociatedValues() const noexcept {
    return associatedValues;
}
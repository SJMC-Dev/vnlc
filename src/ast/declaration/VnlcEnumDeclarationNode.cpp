#include "VnlcEnumDeclarationNode.hpp"

VnlcEnumDeclarationNode::VnlcEnumDeclarationNode(
    std::string&& name,
    std::vector<std::string>&& genericParameterNames,
    std::vector<std::unique_ptr<VnlcEnumMemberDeclarationNode>>&& memberDeclarations,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcTypeDeclarationNode(firstToken, lastToken),
      name(std::move(name)),
      memberDeclarations(std::move(memberDeclarations)),
      genericParameterNames(std::move(genericParameterNames)) {}

VnlcEnumDeclarationNode::VnlcEnumDeclarationNode(
    std::string&& name,
    std::vector<std::string>&& genericParameterNames,
    std::vector<std::unique_ptr<VnlcEnumMemberDeclarationNode>>&& memberDeclarations,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken,
    std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
) noexcept
    : VnlcTypeDeclarationNode(firstToken, lastToken, std::move(metadataTerms)),
      name(std::move(name)),
      memberDeclarations(std::move(memberDeclarations)),
      genericParameterNames(std::move(genericParameterNames)) {}

std::string_view VnlcEnumDeclarationNode::getName() const noexcept {
    return name;
}

const std::vector<std::unique_ptr<VnlcEnumMemberDeclarationNode>>& VnlcEnumDeclarationNode::getMemberDeclarations() const noexcept {
    return memberDeclarations;
}

const std::vector<std::string>& VnlcEnumDeclarationNode::getGenericParameterNames() const noexcept {
    return genericParameterNames;
}
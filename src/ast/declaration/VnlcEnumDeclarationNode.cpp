#include "VnlcEnumDeclarationNode.hpp"

VnlcEnumDeclarationNode::VnlcEnumDeclarationNode(
    std::string&& name,
    std::vector<std::unique_ptr<VnlcEnumMemberDeclarationNode>>&& memberDeclarations,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken),
      name(std::move(name)),
      memberDeclarations(std::move(memberDeclarations)) {}

VnlcEnumDeclarationNode::VnlcEnumDeclarationNode(
    std::string&& name,
    std::vector<std::unique_ptr<VnlcEnumMemberDeclarationNode>>&& memberDeclarations,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken,
    bool includeMetadata,
    std::optional<bool> deprecated,
    std::optional<bool> experimental,
    std::optional<bool> nowarnings,
    std::optional<std::string>&& minGameVersion,
    std::optional<std::string>&& maxGameVersion
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken, includeMetadata, deprecated, experimental, nowarnings, std::move(minGameVersion), std::move(maxGameVersion)),
      name(std::move(name)),
      memberDeclarations(std::move(memberDeclarations)) {}

std::string_view VnlcEnumDeclarationNode::getName() const noexcept {
    return name;
}

const std::vector<std::unique_ptr<VnlcEnumMemberDeclarationNode>>& VnlcEnumDeclarationNode::getMemberDeclarations() const noexcept {
    return memberDeclarations;
}
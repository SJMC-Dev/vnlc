#include "VnlcEnumMemberDeclarationNode.hpp"

VnlcEnumMemberDeclarationNode::VnlcEnumMemberDeclarationNode(
    std::string&& name,
    std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>>&& associatedValues,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken),
      name(std::move(name)),
      associatedValues(std::move(associatedValues)) {}

VnlcEnumMemberDeclarationNode::VnlcEnumMemberDeclarationNode(
    std::string&& name,
    std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>>&& associatedValues,
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
      associatedValues(std::move(associatedValues)) {}

std::string_view VnlcEnumMemberDeclarationNode::getName() const noexcept {
    return name;
}

const std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>>& VnlcEnumMemberDeclarationNode::getAssociatedValues() const noexcept {
    return associatedValues;
}
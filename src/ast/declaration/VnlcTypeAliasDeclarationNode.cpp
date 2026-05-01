#include "VnlcTypeAliasDeclarationNode.hpp"

VnlcTypeAliasDeclarationNode::VnlcTypeAliasDeclarationNode(
    std::string&& aliasName,
    std::vector<std::string>&& originalNameParts,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken),
      aliasName(std::move(aliasName)),
      originalNameParts(std::move(originalNameParts)) {}

VnlcTypeAliasDeclarationNode::VnlcTypeAliasDeclarationNode(
    std::string&& aliasName,
    std::vector<std::string>&& originalNameParts,
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
      aliasName(std::move(aliasName)),
      originalNameParts(std::move(originalNameParts)) {}

std::string_view VnlcTypeAliasDeclarationNode::getAliasName() const noexcept {
    return aliasName;
}

const std::vector<std::string>& VnlcTypeAliasDeclarationNode::getOriginalNameParts() const noexcept {
    return originalNameParts;
}
#include "VnlcTypeAliasDeclarationNode.hpp"

VnlcTypeAliasDeclarationNode::VnlcTypeAliasDeclarationNode(
    std::string&& aliasName,
    std::unique_ptr<VnlcTypeNode>&& originalType,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcTypeDeclarationNode(firstToken, lastToken),
      aliasName(std::move(aliasName)),
      originalType(std::move(originalType)) {}

VnlcTypeAliasDeclarationNode::VnlcTypeAliasDeclarationNode(
    std::string&& aliasName,
    std::unique_ptr<VnlcTypeNode>&& originalType,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken,
    bool includeMetadata,
    std::optional<bool> deprecated,
    std::optional<bool> experimental,
    std::optional<bool> nowarnings,
    std::optional<std::string>&& minGameVersion,
    std::optional<std::string>&& maxGameVersion
) noexcept
    : VnlcTypeDeclarationNode(firstToken, lastToken, includeMetadata, deprecated, experimental, nowarnings, std::move(minGameVersion), std::move(maxGameVersion)),
      aliasName(std::move(aliasName)),
      originalType(std::move(originalType)) {}

std::string_view VnlcTypeAliasDeclarationNode::getAliasName() const noexcept {
    return aliasName;
}

const VnlcTypeNode& VnlcTypeAliasDeclarationNode::getOriginalType() const noexcept {
    return *originalType;
}
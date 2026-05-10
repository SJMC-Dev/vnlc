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
    std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
) noexcept
    : VnlcTypeDeclarationNode(firstToken, lastToken, std::move(metadataTerms)),
      aliasName(std::move(aliasName)),
      originalType(std::move(originalType)) {}

std::string_view VnlcTypeAliasDeclarationNode::getAliasName() const noexcept {
    return aliasName;
}

const VnlcTypeNode& VnlcTypeAliasDeclarationNode::getOriginalType() const noexcept {
    return *originalType;
}
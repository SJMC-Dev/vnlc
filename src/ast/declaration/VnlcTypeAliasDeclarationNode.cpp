#include "VnlcTypeAliasDeclarationNode.hpp"

VnlcTypeAliasDeclarationNode::VnlcTypeAliasDeclarationNode(
    std::string&& aliasName,
    std::vector<std::string>&& genericParameterNames,
    std::unique_ptr<VnlcTypeNode>&& originalType,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcTypeDeclarationNode(firstToken, lastToken),
      aliasName(std::move(aliasName)),
      genericParameterNames(std::move(genericParameterNames)),
      originalType(std::move(originalType)) {}

VnlcTypeAliasDeclarationNode::VnlcTypeAliasDeclarationNode(
    std::string&& aliasName,
    std::vector<std::string>&& genericParameterNames,
    std::unique_ptr<VnlcTypeNode>&& originalType,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken,
    std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
) noexcept
    : VnlcTypeDeclarationNode(firstToken, lastToken, std::move(metadataTerms)),
      aliasName(std::move(aliasName)),
      genericParameterNames(std::move(genericParameterNames)),
      originalType(std::move(originalType)) {}

std::string_view VnlcTypeAliasDeclarationNode::getAliasName() const noexcept {
    return aliasName;
}

const std::vector<std::string>& VnlcTypeAliasDeclarationNode::getGenericParameterNames() const noexcept {
    return genericParameterNames;
}

const VnlcTypeNode& VnlcTypeAliasDeclarationNode::getOriginalType() const noexcept {
    return *originalType;
}
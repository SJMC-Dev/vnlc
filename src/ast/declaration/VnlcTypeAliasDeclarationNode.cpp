#include "VnlcTypeAliasDeclarationNode.hpp"

VnlcTypeAliasDeclarationNode::VnlcTypeAliasDeclarationNode(
    std::unique_ptr<VnlcIdentifierNode>&& aliasName,
    std::vector<std::unique_ptr<VnlcIdentifierNode>>&& genericParameterNames,
    std::unique_ptr<VnlcTypeNode>&& originalType,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcTypeDeclarationNode(firstToken, lastToken),
      aliasName(std::move(aliasName)),
      genericParameterNames(std::move(genericParameterNames)),
      originalType(std::move(originalType)) {}

VnlcTypeAliasDeclarationNode::VnlcTypeAliasDeclarationNode(
    std::unique_ptr<VnlcIdentifierNode>&& aliasName,
    std::vector<std::unique_ptr<VnlcIdentifierNode>>&& genericParameterNames,
    std::unique_ptr<VnlcTypeNode>&& originalType,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken,
    std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
) noexcept
    : VnlcTypeDeclarationNode(firstToken, lastToken, std::move(metadataTerms)),
      aliasName(std::move(aliasName)),
      genericParameterNames(std::move(genericParameterNames)),
      originalType(std::move(originalType)) {}

const VnlcIdentifierNode& VnlcTypeAliasDeclarationNode::getAliasName() const noexcept {
    return *aliasName;
}

const std::vector<std::unique_ptr<VnlcIdentifierNode>>& VnlcTypeAliasDeclarationNode::getGenericParameterNames() const noexcept {
    return genericParameterNames;
}

const VnlcTypeNode& VnlcTypeAliasDeclarationNode::getOriginalType() const noexcept {
    return *originalType;
}
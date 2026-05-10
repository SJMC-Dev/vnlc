#include "VnlcDeclarationNode.hpp"

VnlcDeclarationNode::VnlcDeclarationNode(const VnlcToken& firstToken, const VnlcToken& lastToken) : VnlcAstNode(firstToken, lastToken), includeMetadata(false), metadataTerms() {}

VnlcDeclarationNode::VnlcDeclarationNode(const VnlcToken& firstToken, const VnlcToken& lastToken, std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms) noexcept
    : VnlcAstNode(firstToken, lastToken),
      includeMetadata(true),
      metadataTerms(std::move(metadataTerms)) {}

const bool VnlcDeclarationNode::doesIncludeMetadata() const noexcept {
    return includeMetadata;
}

const std::vector<VnlcDeclarationItem::MetadataTerm>& VnlcDeclarationNode::getMetadataTerms() const noexcept {
    return metadataTerms;
}
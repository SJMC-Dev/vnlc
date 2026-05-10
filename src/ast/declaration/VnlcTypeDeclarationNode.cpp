#include "VnlcTypeDeclarationNode.hpp"

VnlcTypeDeclarationNode::VnlcTypeDeclarationNode(const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept : VnlcDeclarationNode(firstToken, lastToken) {}

VnlcTypeDeclarationNode::VnlcTypeDeclarationNode(const VnlcToken& firstToken, const VnlcToken& lastToken, std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms) noexcept
    : VnlcDeclarationNode(firstToken, lastToken, std::move(metadataTerms)) {}
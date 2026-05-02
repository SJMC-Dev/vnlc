#include "VnlcTypeDeclarationNode.hpp"

VnlcTypeDeclarationNode::VnlcTypeDeclarationNode(const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept : VnlcDeclarationNode(firstToken, lastToken) {}

VnlcTypeDeclarationNode::VnlcTypeDeclarationNode(
    const VnlcToken& firstToken,
    const VnlcToken& lastToken,
    bool includeMetadata,
    std::optional<bool> deprecated,
    std::optional<bool> experimental,
    std::optional<bool> nowarnings,
    std::optional<std::string>&& minGameVersion,
    std::optional<std::string>&& maxGameVersion
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken, includeMetadata, deprecated, experimental, nowarnings, std::move(minGameVersion), std::move(maxGameVersion)) {}
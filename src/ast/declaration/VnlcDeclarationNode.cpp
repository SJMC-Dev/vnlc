#include "VnlcDeclarationNode.hpp"

VnlcDeclarationNode::VnlcDeclarationNode(const VnlcToken& firstToken, const VnlcToken& lastToken)
    : VnlcAstNode(firstToken, lastToken),
      includeMetadata(false),
      deprecated(std::nullopt),
      experimental(std::nullopt),
      nowarnings(std::nullopt),
      minGameVersion(std::nullopt),
      maxGameVersion(std::nullopt) {}

VnlcDeclarationNode::VnlcDeclarationNode(
    const VnlcToken& firstToken,
    const VnlcToken& lastToken,
    bool includeMetadata,
    std::optional<bool> deprecated,
    std::optional<bool> experimental,
    std::optional<bool> nowarnings,
    std::optional<std::string>&& minGameVersion,
    std::optional<std::string>&& maxGameVersion
) noexcept
    : VnlcAstNode(firstToken, lastToken),
      includeMetadata(includeMetadata),
      deprecated(deprecated),
      experimental(experimental),
      nowarnings(nowarnings),
      minGameVersion(std::move(minGameVersion)),
      maxGameVersion(std::move(maxGameVersion)) {}
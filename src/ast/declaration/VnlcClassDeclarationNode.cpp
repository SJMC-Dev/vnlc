#include "VnlcClassDeclarationNode.hpp"

VnlcClassDeclarationNode::VnlcClassDeclarationNode(
    bool final,
    std::string&& name,
    std::optional<std::unique_ptr<VnlcTypeNode>>&& baseClass,
    std::vector<std::unique_ptr<VnlcTypeNode>>&& implementedInterfaces,
    std::vector<std::string>&& genericParameterNames,
    std::vector<std::unique_ptr<VnlcDeclarationNode>>&& memberDeclarations,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcTypeDeclarationNode(firstToken, lastToken),
      final(final),
      name(std::move(name)),
      baseClass(std::move(baseClass)),
      implementedInterfaces(std::move(implementedInterfaces)),
      genericParameterNames(std::move(genericParameterNames)),
      memberDeclarations(std::move(memberDeclarations)) {}

VnlcClassDeclarationNode::VnlcClassDeclarationNode(
    bool final,
    std::string&& name,
    std::optional<std::unique_ptr<VnlcTypeNode>>&& baseClass,
    std::vector<std::unique_ptr<VnlcTypeNode>>&& implementedInterfaces,
    std::vector<std::string>&& genericParameterNames,
    std::vector<std::unique_ptr<VnlcDeclarationNode>>&& memberDeclarations,
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
      final(final),
      name(std::move(name)),
      baseClass(std::move(baseClass)),
      implementedInterfaces(std::move(implementedInterfaces)),
      genericParameterNames(std::move(genericParameterNames)),
      memberDeclarations(std::move(memberDeclarations)) {}

const bool VnlcClassDeclarationNode::isFinal() const noexcept {
    return final;
}

std::string_view VnlcClassDeclarationNode::getName() const noexcept {
    return name;
}

const std::optional<std::unique_ptr<VnlcTypeNode>>& VnlcClassDeclarationNode::getBaseClass() const noexcept {
    return baseClass;
}

const std::vector<std::unique_ptr<VnlcTypeNode>>& VnlcClassDeclarationNode::getImplementedInterfaces() const noexcept {
    return implementedInterfaces;
}

const std::vector<std::string>& VnlcClassDeclarationNode::getGenericParameterNames() const noexcept {
    return genericParameterNames;
}

const std::vector<std::unique_ptr<VnlcDeclarationNode>>& VnlcClassDeclarationNode::getMemberDeclarations() const noexcept {
    return memberDeclarations;
}
#include "VnlcClassDeclarationNode.hpp"

VnlcClassDeclarationNode::VnlcClassDeclarationNode(
    bool final,
    std::string&& name,
    std::vector<std::string>&& baseClassNameParts,
    std::vector<std::vector<std::string>>&& implementedInterfaceNamePartsList,
    std::vector<std::string>&& genericParameterNames,
    std::vector<std::unique_ptr<VnlcDeclarationNode>>&& memberDeclarations,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcTypeDeclarationNode(firstToken, lastToken),
      final(final),
      name(std::move(name)),
      baseClassNameParts(std::move(baseClassNameParts)),
      implementedInterfaceNamePartsList(std::move(implementedInterfaceNamePartsList)),
      genericParameterNames(std::move(genericParameterNames)),
      memberDeclarations(std::move(memberDeclarations)) {}

VnlcClassDeclarationNode::VnlcClassDeclarationNode(
    bool final,
    std::string&& name,
    std::vector<std::string>&& baseClassNameParts,
    std::vector<std::vector<std::string>>&& implementedInterfaceNamePartsList,
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
      baseClassNameParts(std::move(baseClassNameParts)),
      implementedInterfaceNamePartsList(std::move(implementedInterfaceNamePartsList)),
      genericParameterNames(std::move(genericParameterNames)),
      memberDeclarations(std::move(memberDeclarations)) {}

bool VnlcClassDeclarationNode::isFinal() const noexcept {
    return final;
}

std::string_view VnlcClassDeclarationNode::getName() const noexcept {
    return name;
}

const std::vector<std::string>& VnlcClassDeclarationNode::getBaseClassNameParts() const noexcept {
    return baseClassNameParts;
}

const std::vector<std::vector<std::string>>& VnlcClassDeclarationNode::getImplementedInterfaceNamePartsList() const noexcept {
    return implementedInterfaceNamePartsList;
}

const std::vector<std::string>& VnlcClassDeclarationNode::getGenericParameterNames() const noexcept {
    return genericParameterNames;
}

const std::vector<std::unique_ptr<VnlcDeclarationNode>>& VnlcClassDeclarationNode::getMemberDeclarations() const noexcept {
    return memberDeclarations;
}
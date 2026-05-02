#include "VnlcInterfaceDeclarationNode.hpp"

VnlcInterfaceDeclarationNode::VnlcInterfaceDeclarationNode(
    std::string&& name,
    std::vector<std::string>&& genericParameterNames,
    std::vector<std::unique_ptr<VnlcFunctionDeclarationNode>>&& methodDeclarations,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken),
      name(std::move(name)),
      genericParameterNames(std::move(genericParameterNames)),
      methodDeclarations(std::move(methodDeclarations)) {}

VnlcInterfaceDeclarationNode::VnlcInterfaceDeclarationNode(
    std::string&& name,
    std::vector<std::string>&& genericParameterNames,
    std::vector<std::unique_ptr<VnlcFunctionDeclarationNode>>&& methodDeclarations,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken,
    bool includeMetadata,
    std::optional<bool> deprecated,
    std::optional<bool> experimental,
    std::optional<bool> nowarnings,
    std::optional<std::string>&& minGameVersion,
    std::optional<std::string>&& maxGameVersion
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken, includeMetadata, deprecated, experimental, nowarnings, std::move(minGameVersion), std::move(maxGameVersion)),
      name(std::move(name)),
      genericParameterNames(std::move(genericParameterNames)),
      methodDeclarations(std::move(methodDeclarations)) {}

std::string_view VnlcInterfaceDeclarationNode::getName() const noexcept {
    return name;
}

const std::vector<std::string>& VnlcInterfaceDeclarationNode::getGenericParameterNames() const noexcept {
    return genericParameterNames;
}

const std::vector<std::unique_ptr<VnlcFunctionDeclarationNode>>& VnlcInterfaceDeclarationNode::getMethodDeclarations() const noexcept {
    return methodDeclarations;
}
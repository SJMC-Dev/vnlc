#include "VnlcInterfaceDeclarationNode.hpp"

VnlcInterfaceDeclarationNode::VnlcInterfaceDeclarationNode(
    std::unique_ptr<VnlcIdentifierNode>&& name,
    std::vector<std::unique_ptr<VnlcIdentifierNode>>&& genericParameterNames,
    std::vector<std::unique_ptr<VnlcFunctionDeclarationNode>>&& methodDeclarations,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcTypeDeclarationNode(firstToken, lastToken),
      name(std::move(name)),
      genericParameterNames(std::move(genericParameterNames)),
      methodDeclarations(std::move(methodDeclarations)) {}

VnlcInterfaceDeclarationNode::VnlcInterfaceDeclarationNode(
    std::unique_ptr<VnlcIdentifierNode>&& name,
    std::vector<std::unique_ptr<VnlcIdentifierNode>>&& genericParameterNames,
    std::vector<std::unique_ptr<VnlcFunctionDeclarationNode>>&& methodDeclarations,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken,
    std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
) noexcept
    : VnlcTypeDeclarationNode(firstToken, lastToken, std::move(metadataTerms)),
      name(std::move(name)),
      genericParameterNames(std::move(genericParameterNames)),
      methodDeclarations(std::move(methodDeclarations)) {}

const VnlcIdentifierNode& VnlcInterfaceDeclarationNode::getName() const noexcept {
    return *name;
}

const std::vector<std::unique_ptr<VnlcIdentifierNode>>& VnlcInterfaceDeclarationNode::getGenericParameterNames() const noexcept {
    return genericParameterNames;
}

const std::vector<std::unique_ptr<VnlcFunctionDeclarationNode>>& VnlcInterfaceDeclarationNode::getMethodDeclarations() const noexcept {
    return methodDeclarations;
}
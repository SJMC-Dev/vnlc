#include "VnlcModuleNode.hpp"

VnlcModuleNode::VnlcModuleNode(
    std::string&& name,
    std::string&& fullName,
    std::vector<std::unique_ptr<VnlcImportDeclarationNode>>&& importDeclarations,
    std::vector<std::unique_ptr<VnlcDeclarationNode>>&& topIdentifierDeclarations,
    std::vector<std::unique_ptr<VnlcExportDeclarationNode>>&& exportDeclarations,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcAstNode(firstToken, lastToken),
      name(std::move(name)),
      fullName(std::move(fullName)),
      importDeclarations(std::move(importDeclarations)),
      topIdentifierDeclarations(std::move(topIdentifierDeclarations)),
      exportDeclarations(std::move(exportDeclarations)) {}

std::string_view VnlcModuleNode::getName() const noexcept {
    return name;
}

std::string_view VnlcModuleNode::getFullName() const noexcept {
    return fullName;
}

const std::vector<std::unique_ptr<VnlcImportDeclarationNode>>& VnlcModuleNode::getImportDeclarations() const noexcept {
    return importDeclarations;
}

const std::vector<std::unique_ptr<VnlcDeclarationNode>>& VnlcModuleNode::getTopIdentifierDeclarations() const noexcept {
    return topIdentifierDeclarations;
}

const std::vector<std::unique_ptr<VnlcExportDeclarationNode>>& VnlcModuleNode::getExportDeclarations() const noexcept {
    return exportDeclarations;
}
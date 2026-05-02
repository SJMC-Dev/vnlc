#include "VnlcExportDeclarationNode.hpp"

VnlcExportDeclarationNode::VnlcExportDeclarationNode(std::vector<VnlcExportDeclarationItem>&& namesListWithAliases, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcDeclarationNode(firstToken, lastToken),
      namesListWithAliases(std::move(namesListWithAliases)) {}

const std::vector<VnlcExportDeclarationItem>& VnlcExportDeclarationNode::getNamesListWithAliases() const noexcept {
    return namesListWithAliases;
}
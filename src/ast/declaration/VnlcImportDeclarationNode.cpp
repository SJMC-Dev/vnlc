#include "VnlcImportDeclarationNode.hpp"

VnlcImportDeclarationNode::VnlcImportDeclarationNode(std::unique_ptr<VnlcImportDeclarationItem>&& paths, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept
    : VnlcDeclarationNode(firstToken, lastToken),
      paths(std::move(paths)) {}

const VnlcImportDeclarationItem& VnlcImportDeclarationNode::getNamePartsListWithAliases() const noexcept {
    return *paths;
}
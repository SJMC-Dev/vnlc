#include "VnlcImportDeclarationNode.hpp"

VnlcImportDeclarationNode::VnlcImportDeclarationNode(
    bool relative,
    std::vector<VnlcImportDeclarationItem>&& namePartsListWithAliases,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken),
      relative(relative),
      namePartsListWithAliases(std::move(namePartsListWithAliases)){}

const bool VnlcImportDeclarationNode::isRelative() const noexcept {
    return relative;
}

const std::vector<VnlcImportDeclarationItem>& VnlcImportDeclarationNode::getNamePartsListWithAliases() const noexcept {
    return namePartsListWithAliases;
}
#include "VnlcImportDeclarationNode.hpp"

VnlcImportDeclarationNode::VnlcImportDeclarationNode(
    bool relative,
    std::vector<VnlcImportDeclarationItem>&& namePartsListWithAliases,
    std::vector<std::vector<std::string>>&& wildcardImportNamePartsList,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken),
      relative(relative),
      namePartsListWithAliases(std::move(namePartsListWithAliases)),
      wildcardImportNamePartsList(std::move(wildcardImportNamePartsList)) {}

bool VnlcImportDeclarationNode::isRelative() const noexcept {
    return relative;
}

const std::vector<VnlcImportDeclarationItem>& VnlcImportDeclarationNode::getNamePartsListWithAliases() const noexcept {
    return namePartsListWithAliases;
}

const std::vector<std::vector<std::string>>& VnlcImportDeclarationNode::getWildcardImportNamePartsList() const noexcept {
    return wildcardImportNamePartsList;
}
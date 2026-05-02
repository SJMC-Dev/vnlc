#pragma once

#ifndef VNLC_IMPORT_DECLARATION_NODE_HPP
#define VNLC_IMPORT_DECLARATION_NODE_HPP

#include "VnlcDeclarationNode.hpp"
#include "VnlcImportDeclarationItem.hpp"
#include <string>
#include <vector>

class VnlcImportDeclarationNode : public VnlcDeclarationNode {
private:
    VnlcImportDeclarationNode() = delete;

    bool relative;
    std::vector<VnlcImportDeclarationItem> namePartsListWithAliases;
    std::vector<std::vector<std::string>> wildcardImportNamePartsList;

public:
    VnlcImportDeclarationNode(
        bool relative,
        std::vector<VnlcImportDeclarationItem>&& namePartsListWithAliases,
        std::vector<std::vector<std::string>>&& wildcardImportNamePartsList,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    [[nodiscard]] const bool isRelative() const noexcept;
    [[nodiscard]] const std::vector<VnlcImportDeclarationItem>& getNamePartsListWithAliases() const noexcept;
    [[nodiscard]] const std::vector<std::vector<std::string>>& getWildcardImportNamePartsList() const noexcept;
};

#endif // VNLC_IMPORT_DECLARATION_NODE_HPP
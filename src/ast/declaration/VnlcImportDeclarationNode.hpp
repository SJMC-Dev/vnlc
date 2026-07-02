#ifndef VNLC_IMPORT_DECLARATION_NODE_HPP
#define VNLC_IMPORT_DECLARATION_NODE_HPP

#include "VnlcDeclarationNode.hpp"
#include "VnlcImportDeclarationItem.hpp"
#include <vector>

class VnlcImportDeclarationNode : public VnlcDeclarationNode {
private:
    VnlcImportDeclarationNode() = delete;

    bool relative;
    std::vector<VnlcImportDeclarationItem> namePartsListWithAliases;

public:
    VnlcImportDeclarationNode(bool relative, std::vector<VnlcImportDeclarationItem>&& namePartsListWithAliases, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    [[nodiscard]] const bool isRelative() const noexcept;
    [[nodiscard]] const std::vector<VnlcImportDeclarationItem>& getNamePartsListWithAliases() const noexcept;
};

#endif // VNLC_IMPORT_DECLARATION_NODE_HPP
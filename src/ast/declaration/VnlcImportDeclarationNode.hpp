#ifndef VNLC_IMPORT_DECLARATION_NODE_HPP
#define VNLC_IMPORT_DECLARATION_NODE_HPP

#include "VnlcDeclarationNode.hpp"
#include "VnlcImportDeclarationItem.hpp"

class VnlcImportDeclarationNode : public VnlcDeclarationNode {
private:
    VnlcImportDeclarationNode() = delete;

    std::unique_ptr<VnlcImportDeclarationItem> paths;

public:
    VnlcImportDeclarationNode(std::unique_ptr<VnlcImportDeclarationItem>&& paths, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    [[nodiscard]] const VnlcImportDeclarationItem& getNamePartsListWithAliases() const noexcept;
};

#endif // VNLC_IMPORT_DECLARATION_NODE_HPP
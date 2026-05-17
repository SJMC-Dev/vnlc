#pragma once

#ifndef VNLC_TYPE_ALIAS_DECLARATION_NODE_HPP
#define VNLC_TYPE_ALIAS_DECLARATION_NODE_HPP

#include "../other/VnlcTypeNode.hpp"
#include "VnlcTypeDeclarationNode.hpp"
#include <memory>
#include <vector>

class VnlcTypeAliasDeclarationNode : public VnlcTypeDeclarationNode {
private:
    VnlcTypeAliasDeclarationNode() = delete;

    std::string aliasName;
    std::vector<std::string> genericParameterNames;
    std::unique_ptr<VnlcTypeNode> originalType;

public:
    VnlcTypeAliasDeclarationNode(
        std::string&& aliasName,
        std::vector<std::string>&& genericParameterNames,
        std::unique_ptr<VnlcTypeNode>&& originalType,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    VnlcTypeAliasDeclarationNode(
        std::string&& aliasName,
        std::vector<std::string>&& genericParameterNames,
        std::unique_ptr<VnlcTypeNode>&& originalType,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken,
        std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
    ) noexcept;

    [[nodiscard]] std::string_view getAliasName() const noexcept;
    [[nodiscard]] const std::vector<std::string>& getGenericParameterNames() const noexcept;
    [[nodiscard]] const VnlcTypeNode& getOriginalType() const noexcept;
};

#endif // VNLC_TYPE_ALIAS_DECLARATION_NODE_HPP
#pragma once

#ifndef VNLC_TYPE_ALIAS_DECLARATION_NODE_HPP
#define VNLC_TYPE_ALIAS_DECLARATION_NODE_HPP

#include "VnlcDeclarationNode.hpp"
#include <vector>

class VnlcTypeAliasDeclarationNode : public VnlcDeclarationNode {
private:
    VnlcTypeAliasDeclarationNode() = delete;

    std::string aliasName;
    std::vector<std::string> originalNameParts;

public:
    VnlcTypeAliasDeclarationNode(std::string&& aliasName, std::vector<std::string>&& originalNameParts, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    VnlcTypeAliasDeclarationNode(
        std::string&& aliasName,
        std::vector<std::string>&& originalNameParts,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken,
        bool includeMetadata,
        std::optional<bool> deprecated,
        std::optional<bool> experimental,
        std::optional<bool> nowarnings,
        std::optional<std::string>&& minGameVersion,
        std::optional<std::string>&& maxGameVersion
    ) noexcept;

    [[nodiscard]] std::string_view getAliasName() const noexcept;
    [[nodiscard]] const std::vector<std::string>& getOriginalNameParts() const noexcept;
};

#endif // VNLC_TYPE_ALIAS_DECLARATION_NODE_HPP
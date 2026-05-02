#pragma once

#ifndef VNLC_ENUM_DECLARATION_NODE_HPP
#define VNLC_ENUM_DECLARATION_NODE_HPP

#include "VnlcEnumMemberDeclarationNode.hpp"
#include "VnlcTypeDeclarationNode.hpp"

class VnlcEnumDeclarationNode : public VnlcTypeDeclarationNode {
private:
    VnlcEnumDeclarationNode() = delete;

    std::string name;
    std::vector<std::string> genericParameterNames;
    std::vector<std::unique_ptr<VnlcEnumMemberDeclarationNode>> memberDeclarations;

public:
    VnlcEnumDeclarationNode(
        std::string&& name,
        std::vector<std::string>&& genericParameterNames,
        std::vector<std::unique_ptr<VnlcEnumMemberDeclarationNode>>&& memberDeclarations,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    VnlcEnumDeclarationNode(
        std::string&& name,
        std::vector<std::string>&& genericParameterNames,
        std::vector<std::unique_ptr<VnlcEnumMemberDeclarationNode>>&& memberDeclarations,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken,
        bool includeMetadata,
        std::optional<bool> deprecated,
        std::optional<bool> experimental,
        std::optional<bool> nowarnings,
        std::optional<std::string>&& minGameVersion,
        std::optional<std::string>&& maxGameVersion
    ) noexcept;

    [[nodiscard]] std::string_view getName() const noexcept;
    [[nodiscard]] const std::vector<std::unique_ptr<VnlcEnumMemberDeclarationNode>>& getMemberDeclarations() const noexcept;
    [[nodiscard]] const std::vector<std::string>& getGenericParameterNames() const noexcept;
};

#endif // VNLC_ENUM_DECLARATION_NODE_HPP
#pragma once

#ifndef VNLC_ENUM_MEMBER_DECLARATION_NODE_HPP
#define VNLC_ENUM_MEMBER_DECLARATION_NODE_HPP

#include "../other/VnlcTypeAnnotationNode.hpp"
#include "VnlcDeclarationNode.hpp"
#include <utility>
#include <vector>

class VnlcEnumMemberDeclarationNode : public VnlcDeclarationNode {
private:
    VnlcEnumMemberDeclarationNode() = delete;

    std::string name;
    std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>> associatedValues;

public:
    VnlcEnumMemberDeclarationNode(
        std::string&& name,
        std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>>&& associatedValues,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    VnlcEnumMemberDeclarationNode(
        std::string&& name,
        std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>>&& associatedValues,
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
    [[nodiscard]] const std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>>& getAssociatedValues() const noexcept;
};

#endif // VNLC_ENUM_MEMBER_DECLARATION_NODE_HPP
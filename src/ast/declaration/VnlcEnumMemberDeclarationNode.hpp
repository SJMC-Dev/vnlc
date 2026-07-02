#ifndef VNLC_ENUM_MEMBER_DECLARATION_NODE_HPP
#define VNLC_ENUM_MEMBER_DECLARATION_NODE_HPP

#include "../type/VnlcTypeAnnotationNode.hpp"
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
        std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
    ) noexcept;

    [[nodiscard]] std::string_view getName() const noexcept;
    [[nodiscard]] const std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>>& getAssociatedValues() const noexcept;
};

#endif // VNLC_ENUM_MEMBER_DECLARATION_NODE_HPP
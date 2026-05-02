#pragma once

#ifndef VNLC_TYPE_DECLARATION_NODE_HPP
#define VNLC_TYPE_DECLARATION_NODE_HPP

#include "VnlcDeclarationNode.hpp"
class VnlcTypeDeclarationNode : public VnlcDeclarationNode {
private:
    VnlcTypeDeclarationNode() = delete;

protected:
    VnlcTypeDeclarationNode(const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;
    VnlcTypeDeclarationNode(
        const VnlcToken& firstToken,
        const VnlcToken& lastToken,
        bool includeMetadata,
        std::optional<bool> deprecated,
        std::optional<bool> experimental,
        std::optional<bool> nowarnings,
        std::optional<std::string>&& minGameVersion,
        std::optional<std::string>&& maxGameVersion
    ) noexcept;
};

#endif // VNLC_TYPE_DECLARATION_NODE_HPP
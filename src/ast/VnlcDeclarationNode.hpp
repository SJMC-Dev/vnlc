#pragma once

#ifndef VNLC_DECLARATION_NODE_HPP
#define VNLC_DECLARATION_NODE_HPP

#include "VnlcAstNode.hpp"
#include <optional>

class VnlcDeclarationNode : public VnlcAstNode {
private:
    VnlcDeclarationNode() = delete;

    bool includeMetadata;
    std::optional<bool> deprecated;
    std::optional<bool> experimental;
    std::optional<bool> nowarnings;
    std::optional<std::string> minGameVersion;
    std::optional<std::string> maxGameVersion;

protected:
    VnlcDeclarationNode(const VnlcToken& firstToken, const VnlcToken& lastToken);
    VnlcDeclarationNode(
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

#endif // VNLC_DECLARATION_NODE_HPP
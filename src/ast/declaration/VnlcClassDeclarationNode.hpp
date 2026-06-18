#pragma once

#ifndef VNLC_CLASS_DECLARATION_NODE_HPP
#define VNLC_CLASS_DECLARATION_NODE_HPP

#include "../type/VnlcTypeNode.hpp"
#include "VnlcTypeDeclarationNode.hpp"
#include <memory>
#include <optional>
#include <string>
#include <vector>

class VnlcClassDeclarationNode : public VnlcTypeDeclarationNode {
private:
    VnlcClassDeclarationNode() = delete;

    bool final;
    std::string name;
    std::optional<std::unique_ptr<VnlcTypeNode>> baseClass;           // nullopt if no base class
    std::vector<std::unique_ptr<VnlcTypeNode>> implementedInterfaces; // empty if no implemented interfaces
    std::vector<std::string> genericParameterNames;
    std::vector<std::unique_ptr<VnlcDeclarationNode>> memberDeclarations;

public:
    VnlcClassDeclarationNode(
        bool final,
        std::string&& name,
        std::optional<std::unique_ptr<VnlcTypeNode>>&& baseClass,
        std::vector<std::unique_ptr<VnlcTypeNode>>&& implementedInterfaces,
        std::vector<std::string>&& genericParameterNames,
        std::vector<std::unique_ptr<VnlcDeclarationNode>>&& memberDeclarations,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    VnlcClassDeclarationNode(
        bool final,
        std::string&& name,
        std::optional<std::unique_ptr<VnlcTypeNode>>&& baseClass,
        std::vector<std::unique_ptr<VnlcTypeNode>>&& implementedInterfaces,
        std::vector<std::string>&& genericParameterNames,
        std::vector<std::unique_ptr<VnlcDeclarationNode>>&& memberDeclarations,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken,
        std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
    ) noexcept;

    [[nodiscard]] const bool isFinal() const noexcept;
    [[nodiscard]] std::string_view getName() const noexcept;
    [[nodiscard]] const std::optional<std::unique_ptr<VnlcTypeNode>>& getBaseClass() const noexcept;
    [[nodiscard]] const std::vector<std::unique_ptr<VnlcTypeNode>>& getImplementedInterfaces() const noexcept;
    [[nodiscard]] const std::vector<std::string>& getGenericParameterNames() const noexcept;
    [[nodiscard]] const std::vector<std::unique_ptr<VnlcDeclarationNode>>& getMemberDeclarations() const noexcept;
};

#endif // VNLC_CLASS_DECLARATION_NODE_HPP
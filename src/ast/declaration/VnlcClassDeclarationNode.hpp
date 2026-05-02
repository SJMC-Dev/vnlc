#pragma once

#ifndef VNLC_CLASS_DECLARATION_NODE_HPP
#define VNLC_CLASS_DECLARATION_NODE_HPP

#include "VnlcTypeDeclarationNode.hpp"
#include <memory>
#include <string>
#include <vector>

class VnlcClassDeclarationNode : public VnlcTypeDeclarationNode {
private:
    VnlcClassDeclarationNode() = delete;

    bool final;
    std::string name;
    std::vector<std::string> baseClassNameParts;
    std::vector<std::vector<std::string>> implementedInterfaceNamePartsList;
    std::vector<std::string> genericParameterNames;
    std::vector<std::unique_ptr<VnlcDeclarationNode>> memberDeclarations;

public:
    VnlcClassDeclarationNode(
        bool final,
        std::string&& name,
        std::vector<std::string>&& baseClassNameParts,
        std::vector<std::vector<std::string>>&& implementedInterfaceNamePartsList,
        std::vector<std::string>&& genericParameterNames,
        std::vector<std::unique_ptr<VnlcDeclarationNode>>&& memberDeclarations,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    VnlcClassDeclarationNode(
        bool final,
        std::string&& name,
        std::vector<std::string>&& baseClassNameParts,
        std::vector<std::vector<std::string>>&& implementedInterfaceNamePartsList,
        std::vector<std::string>&& genericParameterNames,
        std::vector<std::unique_ptr<VnlcDeclarationNode>>&& memberDeclarations,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken,
        bool includeMetadata,
        std::optional<bool> deprecated,
        std::optional<bool> experimental,
        std::optional<bool> nowarnings,
        std::optional<std::string>&& minGameVersion,
        std::optional<std::string>&& maxGameVersion
    ) noexcept;

    [[nodiscard]] bool isFinal() const noexcept;
    [[nodiscard]] std::string_view getName() const noexcept;
    [[nodiscard]] const std::vector<std::string>& getBaseClassNameParts() const noexcept;
    [[nodiscard]] const std::vector<std::vector<std::string>>& getImplementedInterfaceNamePartsList() const noexcept;
    [[nodiscard]] const std::vector<std::string>& getGenericParameterNames() const noexcept;
    [[nodiscard]] const std::vector<std::unique_ptr<VnlcDeclarationNode>>& getMemberDeclarations() const noexcept;
};

#endif // VNLC_CLASS_DECLARATION_NODE_HPP
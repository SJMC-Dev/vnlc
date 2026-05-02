#pragma once

#ifndef VNLC_INTERFACE_DECLARATION_NODE_HPP
#define VNLC_INTERFACE_DECLARATION_NODE_HPP

#include "VnlcDeclarationNode.hpp"
#include "VnlcFunctionDeclarationNode.hpp"
#include <memory>
#include <string>
#include <vector>

class VnlcInterfaceDeclarationNode : public VnlcDeclarationNode {
private:
    VnlcInterfaceDeclarationNode() = delete;

    std::string name;
    std::vector<std::string> genericParameterNames;
    std::vector<std::unique_ptr<VnlcFunctionDeclarationNode>> methodDeclarations;

public:
    VnlcInterfaceDeclarationNode(
        std::string&& name,
        std::vector<std::string>&& genericParameterNames,
        std::vector<std::unique_ptr<VnlcFunctionDeclarationNode>>&& methodDeclarations,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    VnlcInterfaceDeclarationNode(
        std::string&& name,
        std::vector<std::string>&& genericParameterNames,
        std::vector<std::unique_ptr<VnlcFunctionDeclarationNode>>&& methodDeclarations,
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
    [[nodiscard]] const std::vector<std::string>& getGenericParameterNames() const noexcept;
    [[nodiscard]] const std::vector<std::unique_ptr<VnlcFunctionDeclarationNode>>& getMethodDeclarations() const noexcept;
};

#endif // VNLC_INTERFACE_DECLARATION_NODE_HPP
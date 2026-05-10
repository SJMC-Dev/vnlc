#pragma once

#ifndef VNLC_INTERFACE_DECLARATION_NODE_HPP
#define VNLC_INTERFACE_DECLARATION_NODE_HPP

#include "VnlcFunctionDeclarationNode.hpp"
#include "VnlcTypeDeclarationNode.hpp"
#include <memory>
#include <string>
#include <vector>

class VnlcInterfaceDeclarationNode : public VnlcTypeDeclarationNode {
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
        std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
    ) noexcept;

    [[nodiscard]] std::string_view getName() const noexcept;
    [[nodiscard]] const std::vector<std::string>& getGenericParameterNames() const noexcept;
    [[nodiscard]] const std::vector<std::unique_ptr<VnlcFunctionDeclarationNode>>& getMethodDeclarations() const noexcept;
};

#endif // VNLC_INTERFACE_DECLARATION_NODE_HPP
#pragma once

#ifndef VNLC_FUNCTION_DECLARATION_NODE_HPP
#define VNLC_FUNCTION_DECLARATION_NODE_HPP

#include "VnlcDeclarationNode.hpp"
#include "VnlcFunctionDeclarationType.hpp"
#include "VnlcIdentifierNode.hpp"
#include <memory>
#include <optional>

class VnlcFunctionDeclarationNode : public VnlcDeclarationNode {
private:
    VnlcFunctionDeclarationNode() = delete;

    VnlcFunctionDeclarationType::Type type;
    VnlcFunctionDeclarationType::Context context;
    VnlcFunctionDeclarationType::AccessModifier accessModifier;
    VnlcFunctionDeclarationType::Binding binding;

    std::unique_ptr<VnlcIdentifierNode> name;
    // vector<std::unique_ptr<VnlcParameterNode>> parameters;
    // std::optional<std::unique_ptr<VnlcTypeNode>> returnType;
    // std::optional<std::unique_ptr<VnlcStatementNode>> body;

public:
    VnlcFunctionDeclarationNode(const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;
};

#endif // VNLC_FUNCTION_DECLARATION_NODE_HPP
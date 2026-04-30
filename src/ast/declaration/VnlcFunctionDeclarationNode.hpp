#pragma once

#ifndef VNLC_FUNCTION_DECLARATION_NODE_HPP
#define VNLC_FUNCTION_DECLARATION_NODE_HPP

#include "../statement/VnlcBlockStatementNode.hpp"
#include "VnlcDeclarationNode.hpp"
#include "VnlcFunctionDeclarationType.hpp"
#include "../other/VnlcTypeAnnotationNode.hpp"
#include <memory>
#include <optional>
#include <utility>

class VnlcFunctionDeclarationNode : public VnlcDeclarationNode {
private:
    VnlcFunctionDeclarationNode() = delete;

    VnlcFunctionDeclarationType::Kind kind;
    VnlcFunctionDeclarationType::Context context;
    VnlcFunctionDeclarationType::AccessModifier accessModifier;
    VnlcFunctionDeclarationType::Binding binding;

    std::unique_ptr<std::string> name;
    std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>> parameters;
    std::optional<std::unique_ptr<VnlcTypeAnnotationNode>> returnType;
    std::optional<std::unique_ptr<VnlcBlockStatementNode>> body;

public:
    VnlcFunctionDeclarationNode(
        VnlcFunctionDeclarationType::Kind kind,
        VnlcFunctionDeclarationType::Context context,
        VnlcFunctionDeclarationType::AccessModifier accessModifier,
        VnlcFunctionDeclarationType::Binding binding,
        std::unique_ptr<std::string>&& name,
        std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>>&& parameters,
        std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>&& returnType,
        std::optional<std::unique_ptr<VnlcBlockStatementNode>>&& body,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcFunctionDeclarationType::Kind getKind() const noexcept;
    const VnlcFunctionDeclarationType::Context getContext() const noexcept;
    const VnlcFunctionDeclarationType::AccessModifier getAccessModifier() const noexcept;
    const VnlcFunctionDeclarationType::Binding getBinding() const noexcept;
    const std::string& getName() const noexcept;
    const std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>>& getParameters() const noexcept;
    const std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>& getReturnType() const noexcept;
    const std::optional<std::unique_ptr<VnlcBlockStatementNode>>& getBody() const noexcept;
};

#endif // VNLC_FUNCTION_DECLARATION_NODE_HPP
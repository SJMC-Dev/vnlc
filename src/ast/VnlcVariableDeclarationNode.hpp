#pragma once

#ifndef VNLC_VARIABLE_DECLARATION_NODE_HPP
#define VNLC_VARIABLE_DECLARATION_NODE_HPP

#include "VnlcDeclarationNode.hpp"
#include "VnlcExpressionNode.hpp"
#include "VnlcTypeAnnotationNode.hpp"
#include "VnlcVariableDeclarationType.hpp"
#include <memory>

class VnlcVariableDeclarationNode : public VnlcDeclarationNode {
private:
    VnlcVariableDeclarationNode() = delete;

    VnlcVariableDeclarationType type;
    std::string name;
    std::optional<std::unique_ptr<VnlcTypeAnnotationNode>> typeAnnotation;
    std::unique_ptr<VnlcExpressionNode> initializer;

public:
    VnlcVariableDeclarationNode(
        VnlcVariableDeclarationType type,
        std::string&& name,
        std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>&& typeAnnotation,
        std::unique_ptr<VnlcExpressionNode>&& initializer,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcVariableDeclarationType getType() const noexcept;
    const std::string& getName() const noexcept;
    const std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>& getTypeAnnotation() const noexcept;
    const VnlcExpressionNode& getInitializer() const noexcept;
};

#endif // VNLC_VARIABLE_DECLARATION_NODE_HPP
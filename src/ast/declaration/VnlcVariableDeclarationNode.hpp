#ifndef VNLC_VARIABLE_DECLARATION_NODE_HPP
#define VNLC_VARIABLE_DECLARATION_NODE_HPP

#include "../expression/VnlcExpressionNode.hpp"
#include "../type/VnlcTypeAnnotationNode.hpp"
#include "VnlcDeclarationNode.hpp"
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

    VnlcVariableDeclarationNode(
        VnlcVariableDeclarationType type,
        std::string&& name,
        std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>&& typeAnnotation,
        std::unique_ptr<VnlcExpressionNode>&& initializer,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken,
        std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
    ) noexcept;

    [[nodiscard]] const VnlcVariableDeclarationType getType() const noexcept;
    [[nodiscard]] std::string_view getName() const noexcept;
    [[nodiscard]] const std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>& getTypeAnnotation() const noexcept;
    [[nodiscard]] const VnlcExpressionNode& getInitializer() const noexcept;
};

#endif // VNLC_VARIABLE_DECLARATION_NODE_HPP
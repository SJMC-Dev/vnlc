#ifndef VNLC_TYPE_ANNOTATION_NODE_HPP
#define VNLC_TYPE_ANNOTATION_NODE_HPP

#include "../VnlcAstNode.hpp"
#include "VnlcTypeNode.hpp"
#include <memory>

class VnlcTypeAnnotationNode : public VnlcAstNode {
private:
    VnlcTypeAnnotationNode() = delete;

    std::unique_ptr<VnlcTypeNode> typeNode;
    bool readonly;

public:
    VnlcTypeAnnotationNode(bool readonly, std::unique_ptr<VnlcTypeNode>&& typeNode, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    [[nodiscard]] const VnlcTypeNode& getTypeNode() const noexcept;
    [[nodiscard]] const bool isReadonly() const noexcept;
};

#endif // VNLC_TYPE_ANNOTATION_NODE_HPP

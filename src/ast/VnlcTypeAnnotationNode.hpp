#pragma once

#ifndef VNLC_TYPE_ANNOTATION_NODE_HPP
#define VNLC_TYPE_ANNOTATION_NODE_HPP

#include "VnlcAstNode.hpp"
#include "VnlcQualifiedNameNode.hpp"
#include <memory>
#include <vector>

class VnlcTypeAnnotationNode : public VnlcAstNode {
private:
    VnlcTypeAnnotationNode() = delete;

    std::unique_ptr<VnlcQualifiedNameNode> typeName;
    bool readonly;
    bool optional;
    std::vector<std::unique_ptr<VnlcTypeAnnotationNode>> genericArguments;

public:
    VnlcTypeAnnotationNode(
        std::unique_ptr<VnlcQualifiedNameNode>&& typeName,
        bool readonly,
        bool optional,
        std::vector<std::unique_ptr<VnlcTypeAnnotationNode>>&& genericArguments,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    const VnlcQualifiedNameNode& getTypeName() const noexcept;
    bool isReadonly() const noexcept;
    bool isOptional() const noexcept;
    const std::vector<std::unique_ptr<VnlcTypeAnnotationNode>>& getGenericArguments() const noexcept;
};

#endif // VNLC_TYPE_ANNOTATION_NODE_HPP
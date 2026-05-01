#pragma once

#ifndef VNLC_TYPE_ANNOTATION_NODE_HPP
#define VNLC_TYPE_ANNOTATION_NODE_HPP

#include "../VnlcAstNode.hpp"
#include <memory>
#include <vector>

class VnlcTypeAnnotationNode : public VnlcAstNode {
private:
    VnlcTypeAnnotationNode() = delete;

    std::vector<std::string> typeNameParts;
    bool readonly;
    bool optional;
    std::vector<std::unique_ptr<VnlcTypeAnnotationNode>> genericArguments;

public:
    VnlcTypeAnnotationNode(
        std::vector<std::string>&& typeNameParts,
        bool readonly,
        bool optional,
        std::vector<std::unique_ptr<VnlcTypeAnnotationNode>>&& genericArguments,
        const VnlcToken& firstToken,
        const VnlcToken& lastToken
    ) noexcept;

    [[nodiscard]] const std::vector<std::string>& getTypeNameParts() const noexcept;
    [[nodiscard]] bool isReadonly() const noexcept;
    [[nodiscard]] bool isOptional() const noexcept;
    [[nodiscard]] const std::vector<std::unique_ptr<VnlcTypeAnnotationNode>>& getGenericArguments() const noexcept;
};

#endif // VNLC_TYPE_ANNOTATION_NODE_HPP

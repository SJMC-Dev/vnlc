#pragma once

#ifndef VNLC_REFERENCE_TYPE_HPP
#define VNLC_REFERENCE_TYPE_HPP

#include "../../ast/type/VnlcTypeNode.hpp"
#include "VnlcReferenceTypeKind.hpp"
#include "VnlcSemanticType.hpp"

class VnlcReferenceType : public VnlcSemanticType {
private:
    VnlcReferenceTypeKind referenceKind;
    const VnlcTypeNode* const typeNode;

public:
    VnlcReferenceType(bool readonly, VnlcReferenceTypeKind referenceKind, const VnlcTypeNode* typeNode);

    [[nodiscard]] VnlcReferenceTypeKind getReferenceKind() const noexcept;
    [[nodiscard]] const VnlcTypeNode* const getTypeNode() const noexcept;
};

#endif // VNLC_REFERENCE_TYPE_HPP
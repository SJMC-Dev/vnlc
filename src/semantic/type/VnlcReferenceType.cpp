#include "VnlcReferenceType.hpp"

VnlcReferenceType::VnlcReferenceType(bool readonly, VnlcReferenceTypeKind referenceKind, const VnlcTypeNode* typeNode)
    : VnlcSemanticType(readonly),
      referenceKind(referenceKind),
      typeNode(typeNode) {}

VnlcReferenceTypeKind VnlcReferenceType::getReferenceKind() const noexcept {
    return referenceKind;
}

const VnlcTypeNode* const VnlcReferenceType::getTypeNode() const noexcept {
    return typeNode;
}
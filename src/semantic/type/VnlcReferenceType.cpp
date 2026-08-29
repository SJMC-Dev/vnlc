#include "VnlcReferenceType.hpp"

VnlcReferenceType::VnlcReferenceType(VnlcReferenceTypeKind referenceKind, const VnlcTypeNode* typeNode) : VnlcSemanticType(), referenceKind(referenceKind), typeNode(typeNode) {}

VnlcReferenceTypeKind VnlcReferenceType::getReferenceKind() const noexcept {
    return referenceKind;
}

const VnlcTypeNode* const VnlcReferenceType::getTypeNode() const noexcept {
    return typeNode;
}
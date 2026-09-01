#include "VnlcReferenceType.hpp"
#include "VnlcReferenceTypeOrigin.hpp"

VnlcReferenceType::VnlcReferenceType(VnlcReferenceTypeKind referenceKind, std::string_view fullTypeName, const VnlcTypeDeclarationNode* localDeclarationNode)
    : VnlcSemanticType(),
      referenceKind(referenceKind),
      fullTypeName(fullTypeName),
      localDeclarationNode(localDeclarationNode),
      importedDeclarationNode(nullptr) {}

VnlcReferenceType::VnlcReferenceType(VnlcReferenceTypeKind referenceKind, std::string_view fullTypeName, const VnlcImportedIdentifier* importedDeclarationNode)
    : VnlcSemanticType(),
      referenceKind(referenceKind),
      fullTypeName(fullTypeName),
      localDeclarationNode(nullptr),
      importedDeclarationNode(importedDeclarationNode) {}

VnlcReferenceTypeKind VnlcReferenceType::getReferenceKind() const noexcept {
    return referenceKind;
}

VnlcReferenceTypeOrigin VnlcReferenceType::getOrigin() const noexcept {
    return origin;
}

std::string_view VnlcReferenceType::getFullTypeName() const noexcept {
    return fullTypeName;
}
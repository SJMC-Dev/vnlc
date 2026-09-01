#ifndef VNLC_REFERENCE_TYPE_HPP
#define VNLC_REFERENCE_TYPE_HPP

#include "../ast/declaration/VnlcTypeDeclarationNode.hpp"
#include "../vni/import/VnlcImportedIdentifier.hpp"
#include "VnlcReferenceTypeKind.hpp"
#include "VnlcReferenceTypeOrigin.hpp"
#include "VnlcSemanticType.hpp"
#include <string>
#include <string_view>

class VnlcReferenceType : public VnlcSemanticType {
private:
    VnlcReferenceTypeKind referenceKind;
    VnlcReferenceTypeOrigin origin;
    std::string fullTypeName;

    const VnlcTypeDeclarationNode* localDeclarationNode;
    const VnlcImportedIdentifier* importedDeclarationNode;

public:
    VnlcReferenceType(VnlcReferenceTypeKind referenceKind, std::string_view fullTypeName, const VnlcTypeDeclarationNode* localDeclarationNode);
    VnlcReferenceType(VnlcReferenceTypeKind referenceKind, std::string_view fullTypeName, const VnlcImportedIdentifier* importedDeclarationNode);

    [[nodiscard]] std::string_view getFullTypeName() const noexcept override;
    [[nodiscard]] VnlcReferenceTypeKind getReferenceKind() const noexcept;
    [[nodiscard]] VnlcReferenceTypeOrigin getOrigin() const noexcept;

    [[nodiscard]] const VnlcTypeDeclarationNode* getLocalDeclaration() const noexcept;
    [[nodiscard]] const VnlcImportedIdentifier* getImportedDeclaration() const noexcept;
};

#endif // VNLC_REFERENCE_TYPE_HPP
#include "VnlcSemanticAnalysisResult.hpp"
#include "../type/VnlcSemanticType.hpp"
#include <optional>

VnlcSemanticAnalysisResult::VnlcSemanticAnalysisResult(
    std::vector<VnlcDiagnostic>&& errors,
    std::vector<VnlcDiagnostic>&& warnings,
    std::vector<VnlcDiagnostic>&& notes,
    std::unordered_set<std::unique_ptr<VnlcReferenceType>>&& referenceTypes,
    std::unordered_map<const VnlcTypeNode*, const VnlcSemanticType*>&& semanticTypeMap,
    std::unordered_map<const VnlcValueDeclarationNode*, const VnlcSemanticType*>&& referencedValueTypeMap,
    std::unordered_map<const VnlcFunctionDeclarationNode*, const VnlcSemanticType*>&& referencedFunctionReturnTypeMap,
    std::unordered_map<std::string, std::unique_ptr<VnlcImportedPackage>>&& importedPackages
)
    : errors(std::move(errors)),
      warnings(std::move(warnings)),
      notes(std::move(notes)),
      referenceTypes(std::move(referenceTypes)),
      semanticTypeMap(std::move(semanticTypeMap)),
      referencedValueTypeMap(std::move(referencedValueTypeMap)),
      referencedFunctionReturnTypeMap(std::move(referencedFunctionReturnTypeMap)),
      importedPackages(std::move(importedPackages)) {}

bool VnlcSemanticAnalysisResult::hasErrors() const {
    return !errors.empty();
}

bool VnlcSemanticAnalysisResult::hasWarnings() const {
    return !warnings.empty();
}

bool VnlcSemanticAnalysisResult::hasNotes() const {
    return !notes.empty();
}

const std::vector<VnlcDiagnostic>& VnlcSemanticAnalysisResult::getErrors() const {
    return errors;
}

const std::vector<VnlcDiagnostic>& VnlcSemanticAnalysisResult::getWarnings() const {
    return warnings;
}

const std::vector<VnlcDiagnostic>& VnlcSemanticAnalysisResult::getNotes() const {
    return notes;
}

const std::optional<const VnlcSemanticType*> VnlcSemanticAnalysisResult::getSemanticTypeByTypeNode(const VnlcTypeNode* typeNode) const {
    auto it = semanticTypeMap.find(typeNode);
    if (it != semanticTypeMap.end()) {
        return std::make_optional<const VnlcSemanticType*>(it->second);
    }
    return std::nullopt;
}

const std::optional<const VnlcImportedPackage*> VnlcSemanticAnalysisResult::getImportedPackageByName(std::string_view packageName) const {
    auto it = importedPackages.find(std::string(packageName));
    if (it != importedPackages.end()) {
        return std::make_optional<const VnlcImportedPackage*>(it->second.get());
    }
    return std::nullopt;
}

const std::optional<const VnlcSemanticType*> VnlcSemanticAnalysisResult::getReferencedValueType(const VnlcValueDeclarationNode* valueDeclaration) const {
    auto it = referencedValueTypeMap.find(valueDeclaration);
    if (it != referencedValueTypeMap.end()) {
        return std::make_optional<const VnlcSemanticType*>(it->second);
    }
    return std::nullopt;
}

const std::optional<const VnlcSemanticType*> VnlcSemanticAnalysisResult::getReferencedFunctionReturnType(const VnlcFunctionDeclarationNode* functionDeclaration) const {
    auto it = referencedFunctionReturnTypeMap.find(functionDeclaration);
    if (it != referencedFunctionReturnTypeMap.end()) {
        return std::make_optional<const VnlcSemanticType*>(it->second);
    }
    return std::nullopt;
}
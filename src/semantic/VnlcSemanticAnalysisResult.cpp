#include "VnlcSemanticAnalysisResult.hpp"

VnlcSemanticAnalysisResult::VnlcSemanticAnalysisResult(std::vector<VnlcDiagnostic>&& errors, std::vector<VnlcDiagnostic>&& warnings, std::vector<VnlcDiagnostic>&& notes)
    : errors(std::move(errors)),
      warnings(std::move(warnings)),
      notes(std::move(notes)) {}

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
#pragma once

#ifndef VNLC_SEMANTIC_ANALYSIS_RESULT_HPP
#define VNLC_SEMANTIC_ANALYSIS_RESULT_HPP

#include "../diagnostic/VnlcDiagnostic.hpp"
#include <vector>

class VnlcSemanticAnalysisResult {
private:
    std::vector<VnlcDiagnostic> errors;
    std::vector<VnlcDiagnostic> warnings;
    std::vector<VnlcDiagnostic> notes;

public:
    VnlcSemanticAnalysisResult(std::vector<VnlcDiagnostic>&& errors, std::vector<VnlcDiagnostic>&& warnings, std::vector<VnlcDiagnostic>&& notes);
    VnlcSemanticAnalysisResult(const VnlcSemanticAnalysisResult&) = default;
    VnlcSemanticAnalysisResult& operator=(const VnlcSemanticAnalysisResult&) = default;
    VnlcSemanticAnalysisResult(VnlcSemanticAnalysisResult&&) noexcept = default;
    VnlcSemanticAnalysisResult& operator=(VnlcSemanticAnalysisResult&&) noexcept = default;

    [[nodiscard]] bool hasErrors() const;
    [[nodiscard]] bool hasWarnings() const;
    [[nodiscard]] bool hasNotes() const;
    [[nodiscard]] const std::vector<VnlcDiagnostic>& getErrors() const;
    [[nodiscard]] const std::vector<VnlcDiagnostic>& getWarnings() const;
    [[nodiscard]] const std::vector<VnlcDiagnostic>& getNotes() const;
};

#endif // VNLC_SEMANTIC_ANALYSIS_RESULT_HPP
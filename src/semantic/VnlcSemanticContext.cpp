#include "VnlcSemanticContext.hpp"

void VnlcSemanticContext::reportError(const VnlcAstNode& node, std::string_view message) {
    errors.emplace_back(VnlcDiagnosticPhase::SEMANTIC, VnlcDiagnosticSeverity::ERROR, std::string(message), node.locate().first, node.locate().second, node.getOffset(), node.getLength());
}

void VnlcSemanticContext::reportWarning(const VnlcAstNode& node, std::string_view message) {
    warnings.emplace_back(VnlcDiagnosticPhase::SEMANTIC, VnlcDiagnosticSeverity::WARNING, std::string(message), node.locate().first, node.locate().second, node.getOffset(), node.getLength());
}

void VnlcSemanticContext::reportNote(const VnlcAstNode& node, std::string_view message) {
    notes.emplace_back(VnlcDiagnosticPhase::SEMANTIC, VnlcDiagnosticSeverity::NOTE, std::string(message), node.locate().first, node.locate().second, node.getOffset(), node.getLength());
}

void VnlcSemanticContext::pushScope(std::unique_ptr<VnlcScope>&& scope) {
    scopeStack.push_back(std::move(scope));
}

void VnlcSemanticContext::popScope() {
    scopeStack.pop_back();
}

VnlcScope& VnlcSemanticContext::currentScope() {
    return *scopeStack.back();
}
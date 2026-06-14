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

const std::vector<VnlcDiagnostic>& VnlcSemanticContext::getErrors() const noexcept {
    return errors;
}

const std::vector<VnlcDiagnostic>& VnlcSemanticContext::getWarnings() const noexcept {
    return warnings;
}

const std::vector<VnlcDiagnostic>& VnlcSemanticContext::getNotes() const noexcept {
    return notes;
}

const VnlcFunctionDeclarationNode* VnlcSemanticContext::getCurrentFunction() const noexcept {
    return currentFunction;
}

const VnlcClassDeclarationNode* VnlcSemanticContext::getCurrentClass() const noexcept {
    return currentClass;
}

const VnlcInterfaceDeclarationNode* VnlcSemanticContext::getCurrentInterface() const noexcept {
    return currentInterface;
}

const VnlcEnumDeclarationNode* VnlcSemanticContext::getCurrentEnum() const noexcept {
    return currentEnum;
}

unsigned int VnlcSemanticContext::getLoopDepth() const noexcept {
    return loopDepth;
}

unsigned int VnlcSemanticContext::getSwitchDepth() const noexcept {
    return switchDepth;
}

std::tuple<std::vector<VnlcDiagnostic>, std::vector<VnlcDiagnostic>, std::vector<VnlcDiagnostic>> VnlcSemanticContext::takeDiagnostics() {
    return std::make_tuple(std::move(errors), std::move(warnings), std::move(notes));
}
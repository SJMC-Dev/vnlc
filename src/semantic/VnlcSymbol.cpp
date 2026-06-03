#include "VnlcSymbol.hpp"

VnlcSymbol::VnlcSymbol(VnlcSymbolKind kind, std::string name, const VnlcAstNode* declarationNode) : kind(kind), name(std::move(name)), declarationNode(declarationNode) {}

VnlcSymbolKind VnlcSymbol::getKind() const noexcept {
    return kind;
}

std::string_view VnlcSymbol::getName() const noexcept {
    return name;
}

const VnlcAstNode* VnlcSymbol::getDeclarationNode() const noexcept {
    return declarationNode;
}
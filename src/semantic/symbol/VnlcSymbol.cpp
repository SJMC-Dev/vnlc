#include "VnlcSymbol.hpp"

VnlcSymbol::VnlcSymbol(VnlcSymbolKind kind, VnlcSymbolOrigin origin, std::string_view name, const VnlcAstNode* localDeclarationNode)
    : kind(kind),
      origin(origin),
      name(std::move(name)),
      localDeclarationNode(localDeclarationNode) {}

VnlcSymbolKind VnlcSymbol::getKind() const noexcept {
    return kind;
}

VnlcSymbolOrigin VnlcSymbol::getOrigin() const noexcept {
    return origin;
}

std::string_view VnlcSymbol::getName() const noexcept {
    return name;
}

const VnlcAstNode* VnlcSymbol::getLocalDeclarationNode() const noexcept {
    return localDeclarationNode;
}
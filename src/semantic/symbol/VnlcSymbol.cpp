#include "VnlcSymbol.hpp"

VnlcSymbol::VnlcSymbol(VnlcSymbolKind kind, VnlcSymbolOrigin origin, std::string_view name, std::string_view sourceName, const VnlcAstNode* localDeclarationNode)
    : kind(kind),
      origin(origin),
      name(std::move(name)),
      sourceName(std::move(sourceName)),
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

std::string_view VnlcSymbol::getSourceName() const noexcept {
    return sourceName;
}

const VnlcAstNode* VnlcSymbol::getLocalDeclarationNode() const noexcept {
    return localDeclarationNode;
}
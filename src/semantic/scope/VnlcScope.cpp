#include "VnlcScope.hpp"

VnlcScope::VnlcScope(VnlcScopeKind kind, const VnlcScope* parent) noexcept : kind(kind), parent(parent) {}

bool VnlcScope::declare(VnlcSymbol&& symbol) {
    VnlcSymbol existingSymbol = symbols[std::string(symbol.getName())];
    if (existingSymbol.getName() == symbol.getName()) {
        return false;
    } else {
        symbols[std::string(symbol.getName())] = std::move(symbol);
        return true;
    }
}

std::optional<const VnlcSymbol*> VnlcScope::lookupLocal(std::string_view name) const {
    auto it = symbols.find(std::string(name));
    if (it != symbols.end()) {
        return std::optional<const VnlcSymbol*>(&(it->second));
    }
    return std::nullopt;
}

std::optional<const VnlcSymbol*> VnlcScope::lookup(std::string_view name) const {
    const VnlcScope* current = this;
    while (current != nullptr) {
        auto it = current->symbols.find(std::string(name));
        if (it != current->symbols.end()) {
            return std::optional<const VnlcSymbol*>(&(it->second));
        }
        current = current->parent;
    }
    return std::nullopt;
}

VnlcScopeKind VnlcScope::getKind() const noexcept {
    return kind;
}
#include "VnlcScope.hpp"

VnlcScope::VnlcScope(VnlcScopeKind kind, const VnlcScope* parent) noexcept : kind(kind), parent(parent) {}

bool VnlcScope::declare(VnlcSymbol&& symbol) {
    auto& symbolList = symbols[std::string(symbol.getName())];
    for (const auto& existingSymbol : symbolList) {
        if (existingSymbol.getName() == symbol.getName()) {
            return false; // Symbol with the same name already exists in the current scope
        }
    }
    symbolList.push_back(std::move(symbol));
    return true;
}

const std::vector<VnlcSymbol>& VnlcScope::lookupLocal(std::string_view name) const {
    static const std::vector<VnlcSymbol> empty;
    auto it = symbols.find(std::string(name));
    if (it != symbols.end()) {
        return it->second;
    }
    return empty;
}

const std::vector<VnlcSymbol>& VnlcScope::lookup(std::string_view name) const {
    const VnlcScope* current = this;
    while (current != nullptr) {
        auto it = current->symbols.find(std::string(name));
        if (it != current->symbols.end()) {
            return it->second;
        }
        current = current->parent;
    }
    static const std::vector<VnlcSymbol> empty;
    return empty;
}

VnlcScopeKind VnlcScope::getKind() const noexcept {
    return kind;
}
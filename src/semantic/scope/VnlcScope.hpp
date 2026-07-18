#ifndef VNLC_SCOPE_HPP
#define VNLC_SCOPE_HPP

#include "../symbol/VnlcSymbol.hpp"
#include "VnlcScopeKind.hpp"
#include <string>
#include <unordered_map>
#include <optional>

class VnlcScope {

private:
    VnlcScopeKind kind;
    const VnlcScope* parent;
    std::unordered_map<std::string, VnlcSymbol> symbols;

public:
    explicit VnlcScope(VnlcScopeKind kind, const VnlcScope* parent = nullptr) noexcept;

    bool declare(VnlcSymbol&& symbol);
    [[nodiscard]] VnlcScopeKind getKind() const noexcept;
    [[nodiscard]] std::optional<const VnlcSymbol*> lookupLocal(std::string_view name) const;
    [[nodiscard]] std::optional<const VnlcSymbol*> lookup(std::string_view name) const;
};

#endif // VNLC_SCOPE_HPP
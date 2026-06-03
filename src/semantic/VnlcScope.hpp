#pragma once

#ifndef VNLC_SCOPE_HPP
#define VNLC_SCOPE_HPP

#include "VnlcSymbol.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class VnlcScope {
private:
    const VnlcScope* parent;
    std::unordered_map<std::string, std::vector<VnlcSymbol>> symbols;

public:
    explicit VnlcScope(const VnlcScope* parent = nullptr) noexcept;

    bool declare(VnlcSymbol&& symbol);
    [[nodiscard]] const std::vector<VnlcSymbol>& lookupLocal(std::string_view name) const;
    [[nodiscard]] const std::vector<VnlcSymbol>& lookup(std::string_view name) const;
};

#endif // VNLC_SCOPE_HPP
#pragma once

#ifndef VNLC_SYMBOL_HPP
#define VNLC_SYMBOL_HPP

#include "../ast/VnlcAstNode.hpp"
#include "VnlcSymbolKind.hpp"

class VnlcSymbol {
private:
    VnlcSymbolKind kind;
    std::string name;
    const VnlcAstNode* declarationNode;

public:
    VnlcSymbol(VnlcSymbolKind kind, std::string name, const VnlcAstNode* declarationNode);

    [[nodiscard]] VnlcSymbolKind getKind() const noexcept;
    [[nodiscard]] std::string_view getName() const noexcept;
    [[nodiscard]] const VnlcAstNode* getDeclarationNode() const noexcept;
};

#endif // VNLC_SYMBOL_HPP
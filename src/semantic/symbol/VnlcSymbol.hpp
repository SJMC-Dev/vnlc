#ifndef VNLC_SYMBOL_HPP
#define VNLC_SYMBOL_HPP

#include "../../ast/VnlcAstNode.hpp"
#include "VnlcSymbolKind.hpp"
#include "VnlcSymbolOrigin.hpp"
#include <string>
#include <string_view>

class VnlcSymbol {
private:
    VnlcSymbolKind kind;
    VnlcSymbolOrigin origin;
    std::string name;
    std::string sourceName; // The original name of the symbol as it appears in the source code
    const VnlcAstNode* localDeclarationNode;

public:
    VnlcSymbol(VnlcSymbolKind kind, VnlcSymbolOrigin origin, std::string_view name, std::string_view sourceName, const VnlcAstNode* localDeclarationNode);

    [[nodiscard]] VnlcSymbolKind getKind() const noexcept;
    [[nodiscard]] VnlcSymbolOrigin getOrigin() const noexcept;
    [[nodiscard]] std::string_view getName() const noexcept;
    [[nodiscard]] std::string_view getSourceName() const noexcept;
    [[nodiscard]] const VnlcAstNode* getLocalDeclarationNode() const noexcept;
};

#endif // VNLC_SYMBOL_HPP
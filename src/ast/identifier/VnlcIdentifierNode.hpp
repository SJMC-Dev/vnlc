#ifndef VNLC_IDENTIFIER_NODE_HPP
#define VNLC_IDENTIFIER_NODE_HPP

#include "../VnlcAstNode.hpp"
#include <string>
#include <string_view>

class VnlcIdentifierNode : public VnlcAstNode {
private:
    VnlcIdentifierNode() = delete;

    std::string name;

public:
    VnlcIdentifierNode(std::string_view name, const VnlcToken& firstToken, const VnlcToken& lastToken);

    [[nodiscard]] std::string_view getName() const;
};

#endif // VNLC_IDENTIFIER_NODE_HPP
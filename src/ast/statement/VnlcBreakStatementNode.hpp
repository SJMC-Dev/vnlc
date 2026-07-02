#ifndef VNLC_BREAK_STATEMENT_NODE_HPP
#define VNLC_BREAK_STATEMENT_NODE_HPP

#include "VnlcControlFlowStatementNode.hpp"
#include <optional>
#include <string>

class VnlcBreakStatementNode : public VnlcControlFlowStatementNode {
private:
    VnlcBreakStatementNode() = delete;

    std::optional<std::string> label; // nullopt if no label

public:
    VnlcBreakStatementNode(const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;
    VnlcBreakStatementNode(std::string&& label, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    [[nodiscard]] const std::optional<std::string>& getLabel() const noexcept;
};

#endif // VNLC_BREAK_STATEMENT_NODE_HPP
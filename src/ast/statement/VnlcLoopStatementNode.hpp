#ifndef VNLC_LOOP_STATEMENT_NODE_HPP
#define VNLC_LOOP_STATEMENT_NODE_HPP

#include "VnlcControlFlowStatementNode.hpp"
#include <optional>

class VnlcLoopStatementNode : public VnlcControlFlowStatementNode {
private:
    VnlcLoopStatementNode() = delete;

    std::optional<std::string> label; // nullopt if no label

protected:
    VnlcLoopStatementNode(std::optional<std::string>&& label, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

public:
    [[nodiscard]] const std::optional<std::string>& getLabel() const noexcept;
};

#endif // VNLC_LOOP_STATEMENT_NODE_HPP
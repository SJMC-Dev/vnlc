#ifndef VNLC_CONTINUE_STATEMENT_NODE_HPP
#define VNLC_CONTINUE_STATEMENT_NODE_HPP

#include "VnlcControlFlowStatementNode.hpp"
#include <optional>
#include <string>

class VnlcContinueStatementNode : public VnlcControlFlowStatementNode {
private:
    VnlcContinueStatementNode() = delete;

    std::optional<std::string> label; // nullopt if no label

public:
    VnlcContinueStatementNode(const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;
    VnlcContinueStatementNode(std::string&& label, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    [[nodiscard]] const std::optional<std::string>& getLabel() const noexcept;
};

#endif // VNLC_CONTINUE_STATEMENT_NODE_HPP
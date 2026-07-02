#ifndef VNLC_WHILE_STATEMENT_PARSING_CONTEXT_HPP
#define VNLC_WHILE_STATEMENT_PARSING_CONTEXT_HPP

#include <optional>
#include <string>

struct VnlcWhileStatementParsingContext {
    std::optional<std::string> label;
};

#endif // VNLC_WHILE_STATEMENT_PARSING_CONTEXT_HPP
#ifndef VNLC_FOR_STATEMENT_PARSING_CONTEXT_HPP
#define VNLC_FOR_STATEMENT_PARSING_CONTEXT_HPP

#include <optional>
#include <string>

struct VnlcForStatementParsingContext {
    std::optional<std::string> label;
};

#endif // VNLC_FOR_STATEMENT_PARSING_CONTEXT_HPP
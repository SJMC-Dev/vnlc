#include "VnlcRunningModeUtil.hpp"
#include <CLI/CLI.hpp>

VnlcRunningMode VnlcRunningModeUtil::getRunningMode(std::string_view modeLiteral) {
    if (modeLiteral == "compile") {
        return VnlcRunningMode::COMPILE;
    } else if (modeLiteral == "check") {
        return VnlcRunningMode::CHECK;
    } else if (modeLiteral == "lint") {
        return VnlcRunningMode::LINT;
    } else if (modeLiteral == "format") {
        return VnlcRunningMode::FORMAT;
    } else if (modeLiteral == "lsp-server") {
        return VnlcRunningMode::LSP_SERVER;
    } else if (modeLiteral == "dump-ast") {
        return VnlcRunningMode::DUMP_AST;
    } else if (modeLiteral == "dump-ir") {
        return VnlcRunningMode::DUMP_IR;
    } else {
        throw CLI::ValidationError("Invalid running mode: " + std::string(modeLiteral));
    }
}
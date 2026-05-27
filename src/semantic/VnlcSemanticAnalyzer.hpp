#pragma once

#ifndef VNLC_SEMANTIC_ANALYZER_HPP
#define VNLC_SEMANTIC_ANALYZER_HPP

#include "../ast/module/VnlcModuleNode.hpp"
#include <memory>

class VnlcSemanticAnalyzer {
private:
    std::unique_ptr<VnlcModuleNode> module;
};

#endif // VNLC_SEMANTIC_ANALYZER_HPP
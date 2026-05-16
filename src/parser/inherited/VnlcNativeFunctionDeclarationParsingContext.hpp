#pragma once

#ifndef VNLC_NATIVE_FUNCTION_DECLARATION_PARSING_CONTEXT_HPP
#define VNLC_NATIVE_FUNCTION_DECLARATION_PARSING_CONTEXT_HPP

#include "../../ast/declaration/VnlcFunctionDeclarationType.hpp"
#include "../../ast/declaration/VnlcDeclarationItem.hpp"
#include <vector>

struct VnlcNativeFunctionDeclarationParsingContext {
    VnlcFunctionDeclarationType::Context context;

    bool hasMetadata;
    std::vector<VnlcDeclarationItem::MetadataTerm> metadataTerms;
};

#endif // VNLC_NATIVE_FUNCTION_DECLARATION_PARSING_CONTEXT_HPP
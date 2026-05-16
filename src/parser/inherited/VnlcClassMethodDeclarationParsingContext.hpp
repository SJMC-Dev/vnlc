#pragma once

#ifndef VNLC_CLASS_METHOD_DECLARATION_PARSING_CONTEXT_HPP
#define VNLC_CLASS_METHOD_DECLARATION_PARSING_CONTEXT_HPP

#include "../../ast/declaration/VnlcDeclarationItem.hpp"
#include <vector>

struct VnlcClassMethodDeclarationParsingContext {
    bool hasMetadata;
    std::vector<VnlcDeclarationItem::MetadataTerm> metadataTerms;
};

#endif // VNLC_CLASS_METHOD_DECLARATION_PARSING_CONTEXT_HPP
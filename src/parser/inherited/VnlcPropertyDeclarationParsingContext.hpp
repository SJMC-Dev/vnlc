#pragma once

#ifndef VNLC_PROPERTY_DECLARATION_PARSING_CONTEXT_HPP
#define VNLC_PROPERTY_DECLARATION_PARSING_CONTEXT_HPP

#include "../../ast/declaration/VnlcDeclarationItem.hpp"
#include <vector>

struct VnlcPropertyDeclarationParsingContext {
    bool hasMetadata;
    std::vector<VnlcDeclarationItem::MetadataTerm> metadataTerms;
};

#endif // VNLC_PROPERTY_DECLARATION_PARSING_CONTEXT_HPP
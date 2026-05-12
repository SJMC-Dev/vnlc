#pragma once

#ifndef VNLC_ENUM_ASSOCIATED_VALUE_PARSING_RESULT_HPP
#define VNLC_ENUM_ASSOCIATED_VALUE_PARSING_RESULT_HPP

#include "../../ast/other/VnlcTypeAnnotationNode.hpp"
#include <memory>
#include <string>

struct VnlcEnumAssociatedValueParsingResult {
    std::string name;
    std::unique_ptr<VnlcTypeAnnotationNode> typeAnnotation;
};

#endif // VNLC_ENUM_ASSOCIATED_VALUE_PARSING_RESULT_HPP

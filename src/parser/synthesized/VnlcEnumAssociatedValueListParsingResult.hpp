#pragma once

#ifndef VNLC_ENUM_ASSOCIATED_VALUE_LIST_PARSING_RESULT_HPP
#define VNLC_ENUM_ASSOCIATED_VALUE_LIST_PARSING_RESULT_HPP

#include "../../ast/type/VnlcTypeAnnotationNode.hpp"
#include <memory>
#include <string>
#include <vector>

struct VnlcEnumAssociatedValueListParsingResult {
    struct Item {
        std::string name;
        std::unique_ptr<VnlcTypeAnnotationNode> typeAnnotation;
    };

    std::vector<Item> associatedValues;
};

#endif // VNLC_ENUM_ASSOCIATED_VALUE_LIST_PARSING_RESULT_HPP

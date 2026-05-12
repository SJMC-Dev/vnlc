#pragma once

#ifndef VNLC_PARAMETER_LIST_PARSING_RESULT_HPP
#define VNLC_PARAMETER_LIST_PARSING_RESULT_HPP

#include "../../ast/other/VnlcTypeAnnotationNode.hpp"
#include <memory>
#include <string>
#include <vector>

struct VnlcParameterListParsingResult {
    struct Parameter {
        std::string name;
        std::unique_ptr<VnlcTypeAnnotationNode> typeAnnotation;
    };
    std::vector<Parameter> parameters;
};

#endif // VNLC_PARAMETER_LIST_PARSING_RESULT_HPP

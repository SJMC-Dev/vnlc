#ifndef VNLC_PARAMETER_PARSING_RESULT_HPP
#define VNLC_PARAMETER_PARSING_RESULT_HPP

#include "../../ast/type/VnlcTypeAnnotationNode.hpp"
#include <memory>
#include <string>

struct VnlcParameterParsingResult {
    std::string name;
    std::unique_ptr<VnlcTypeAnnotationNode> typeAnnotation;
};

#endif // VNLC_PARAMETER_PARSING_RESULT_HPP

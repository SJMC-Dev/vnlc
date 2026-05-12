#pragma once

#ifndef VNLC_TYPE_ANNOTATION_PARSING_RESULT_HPP
#define VNLC_TYPE_ANNOTATION_PARSING_RESULT_HPP

#include "../../ast/other/VnlcTypeAnnotationNode.hpp"
#include <memory>

struct VnlcTypeAnnotationParsingResult {
    std::unique_ptr<VnlcTypeAnnotationNode> typeAnnotation;
};

#endif // VNLC_TYPE_ANNOTATION_PARSING_RESULT_HPP

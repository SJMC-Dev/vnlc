#pragma once

#ifndef VNLC_FUNCTION_SIGNATURE_PARSING_RESULT_HPP
#define VNLC_FUNCTION_SIGNATURE_PARSING_RESULT_HPP

#include "../../ast/type/VnlcTypeAnnotationNode.hpp"
#include <memory>
#include <optional>
#include <string>
#include <vector>

struct VnlcFunctionSignatureParsingResult {
    std::unique_ptr<std::string> name;
    std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>> parameters;
    std::optional<std::unique_ptr<VnlcTypeAnnotationNode>> returnType;
};

#endif // VNLC_FUNCTION_SIGNATURE_PARSING_RESULT_HPP
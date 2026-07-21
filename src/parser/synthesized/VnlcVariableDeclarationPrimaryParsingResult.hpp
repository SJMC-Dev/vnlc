#ifndef VNLC_VARIABLE_DECLARATION_PRIMARY_PARSING_RESULT_HPP
#define VNLC_VARIABLE_DECLARATION_PRIMARY_PARSING_RESULT_HPP

#include "../../ast/declaration/VnlcValueDeclarationType.hpp"
#include "../../ast/type/VnlcTypeAnnotationNode.hpp"
#include <memory>
#include <optional>

struct VnlcVariableDeclarationPrimaryParsingResult {
    VnlcValueDeclarationType::Kind kind;
    std::string name;
    std::optional<std::unique_ptr<VnlcTypeAnnotationNode>> typeAnnotation;
};

#endif // VNLC_VARIABLE_DECLARATION_PRIMARY_PARSING_RESULT_HPP
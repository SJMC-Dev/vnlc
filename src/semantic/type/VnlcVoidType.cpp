#include "VnlcVoidType.hpp"

VnlcVoidType::VnlcVoidType() : VnlcSemanticType() {}

std::string_view VnlcVoidType::getFullTypeName() const noexcept {
    return "void";
}
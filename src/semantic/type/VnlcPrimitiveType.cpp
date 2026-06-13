#include "VnlcPrimitiveType.hpp"

VnlcPrimitiveType::VnlcPrimitiveType(bool readonly, VnlcPrimitiveTypeKind primitiveKind) : VnlcSemanticType(readonly), primitiveKind(primitiveKind) {}

VnlcPrimitiveTypeKind VnlcPrimitiveType::getPrimitiveKind() const noexcept {
    return primitiveKind;
}
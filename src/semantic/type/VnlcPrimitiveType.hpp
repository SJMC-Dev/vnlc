#ifndef VNLC_PRIMITIVE_TYPE_HPP
#define VNLC_PRIMITIVE_TYPE_HPP

#include "VnlcPrimitiveTypeKind.hpp"
#include "VnlcSemanticType.hpp"

class VnlcPrimitiveType : public VnlcSemanticType {
private:
    VnlcPrimitiveTypeKind primitiveKind;

public:
    VnlcPrimitiveType(bool readonly, VnlcPrimitiveTypeKind primitiveKind);

    [[nodiscard]] VnlcPrimitiveTypeKind getPrimitiveKind() const noexcept;
};

#endif // VNLC_PRIMITIVE_TYPE_HPP
#ifndef VNLC_PRIMITIVE_TYPE_HPP
#define VNLC_PRIMITIVE_TYPE_HPP

#include "VnlcPrimitiveTypeKind.hpp"
#include "VnlcSemanticType.hpp"

class VnlcPrimitiveType : public VnlcSemanticType {
private:
    VnlcPrimitiveTypeKind primitiveKind;

    static const VnlcPrimitiveType* BYTE_TYPE;
    static const VnlcPrimitiveType* SHORT_TYPE;
    static const VnlcPrimitiveType* INT_TYPE;
    static const VnlcPrimitiveType* LONG_TYPE;
    static const VnlcPrimitiveType* FLOAT_TYPE;
    static const VnlcPrimitiveType* DOUBLE_TYPE;
    static const VnlcPrimitiveType* BOOLEAN_TYPE;
    static const VnlcPrimitiveType* STRING_TYPE;

    VnlcPrimitiveType(bool readonly, VnlcPrimitiveTypeKind primitiveKind);

public:
    [[nodiscard]] VnlcPrimitiveTypeKind getPrimitiveKind() const noexcept;

    [[nodiscard]] static const VnlcPrimitiveType* getByteType();
    [[nodiscard]] static const VnlcPrimitiveType* getShortType();
    [[nodiscard]] static const VnlcPrimitiveType* getIntType();
    [[nodiscard]] static const VnlcPrimitiveType* getLongType();
    [[nodiscard]] static const VnlcPrimitiveType* getFloatType();
    [[nodiscard]] static const VnlcPrimitiveType* getDoubleType();
    [[nodiscard]] static const VnlcPrimitiveType* getBooleanType();
    [[nodiscard]] static const VnlcPrimitiveType* getStringType();
};

#endif // VNLC_PRIMITIVE_TYPE_HPP
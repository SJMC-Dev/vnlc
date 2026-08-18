#include "VnlcPrimitiveType.hpp"

VnlcPrimitiveType::VnlcPrimitiveType(bool readonly, VnlcPrimitiveTypeKind primitiveKind) : VnlcSemanticType(readonly), primitiveKind(primitiveKind) {}

VnlcPrimitiveTypeKind VnlcPrimitiveType::getPrimitiveKind() const noexcept {
    return primitiveKind;
}

const VnlcPrimitiveType* VnlcPrimitiveType::BYTE_TYPE = nullptr;
const VnlcPrimitiveType* VnlcPrimitiveType::SHORT_TYPE = nullptr;
const VnlcPrimitiveType* VnlcPrimitiveType::INT_TYPE = nullptr;
const VnlcPrimitiveType* VnlcPrimitiveType::LONG_TYPE = nullptr;
const VnlcPrimitiveType* VnlcPrimitiveType::FLOAT_TYPE = nullptr;
const VnlcPrimitiveType* VnlcPrimitiveType::DOUBLE_TYPE = nullptr;
const VnlcPrimitiveType* VnlcPrimitiveType::BOOLEAN_TYPE = nullptr;
const VnlcPrimitiveType* VnlcPrimitiveType::STRING_TYPE = nullptr;

const VnlcPrimitiveType* VnlcPrimitiveType::getByteType() {
    if (BYTE_TYPE == nullptr) {
        BYTE_TYPE = new VnlcPrimitiveType(false, VnlcPrimitiveTypeKind::BYTE);
    }
    return BYTE_TYPE;
}

const VnlcPrimitiveType* VnlcPrimitiveType::getShortType() {
    if (SHORT_TYPE == nullptr) {
        SHORT_TYPE = new VnlcPrimitiveType(false, VnlcPrimitiveTypeKind::SHORT);
    }
    return SHORT_TYPE;
}

const VnlcPrimitiveType* VnlcPrimitiveType::getIntType() {
    if (INT_TYPE == nullptr) {
        INT_TYPE = new VnlcPrimitiveType(false, VnlcPrimitiveTypeKind::INT);
    }
    return INT_TYPE;
}

const VnlcPrimitiveType* VnlcPrimitiveType::getLongType() {
    if (LONG_TYPE == nullptr) {
        LONG_TYPE = new VnlcPrimitiveType(false, VnlcPrimitiveTypeKind::LONG);
    }
    return LONG_TYPE;
}

const VnlcPrimitiveType* VnlcPrimitiveType::getFloatType() {
    if (FLOAT_TYPE == nullptr) {
        FLOAT_TYPE = new VnlcPrimitiveType(false, VnlcPrimitiveTypeKind::FLOAT);
    }
    return FLOAT_TYPE;
}

const VnlcPrimitiveType* VnlcPrimitiveType::getDoubleType() {
    if (DOUBLE_TYPE == nullptr) {
        DOUBLE_TYPE = new VnlcPrimitiveType(false, VnlcPrimitiveTypeKind::DOUBLE);
    }
    return DOUBLE_TYPE;
}

const VnlcPrimitiveType* VnlcPrimitiveType::getBooleanType() {
    if (BOOLEAN_TYPE == nullptr) {
        BOOLEAN_TYPE = new VnlcPrimitiveType(false, VnlcPrimitiveTypeKind::BOOLEAN);
    }
    return BOOLEAN_TYPE;
}

const VnlcPrimitiveType* VnlcPrimitiveType::getStringType() {
    if (STRING_TYPE == nullptr) {
        STRING_TYPE = new VnlcPrimitiveType(false, VnlcPrimitiveTypeKind::STRING);
    }
    return STRING_TYPE;
}
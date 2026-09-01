#include "VnlcPrimitiveType.hpp"

VnlcPrimitiveType::VnlcPrimitiveType(VnlcPrimitiveTypeKind primitiveKind) : VnlcSemanticType(), primitiveKind(primitiveKind) {}

VnlcPrimitiveTypeKind VnlcPrimitiveType::getPrimitiveKind() const noexcept {
    return primitiveKind;
}

std::string_view VnlcPrimitiveType::getFullTypeName() const noexcept {
    switch (primitiveKind) {
        case VnlcPrimitiveTypeKind::BYTE:
            return "byte";
        case VnlcPrimitiveTypeKind::SHORT:
            return "short";
        case VnlcPrimitiveTypeKind::INT:
            return "int";
        case VnlcPrimitiveTypeKind::LONG:
            return "long";
        case VnlcPrimitiveTypeKind::FLOAT:
            return "float";
        case VnlcPrimitiveTypeKind::DOUBLE:
            return "double";
        case VnlcPrimitiveTypeKind::BOOLEAN:
            return "bool";
        case VnlcPrimitiveTypeKind::STRING:
            return "string";
    }
}

const VnlcPrimitiveType* VnlcPrimitiveType::BYTE_TYPE = nullptr;
const VnlcPrimitiveType* VnlcPrimitiveType::SHORT_TYPE = nullptr;
const VnlcPrimitiveType* VnlcPrimitiveType::INT_TYPE = nullptr;
const VnlcPrimitiveType* VnlcPrimitiveType::LONG_TYPE = nullptr;
const VnlcPrimitiveType* VnlcPrimitiveType::FLOAT_TYPE = nullptr;
const VnlcPrimitiveType* VnlcPrimitiveType::DOUBLE_TYPE = nullptr;
const VnlcPrimitiveType* VnlcPrimitiveType::BOOLEAN_TYPE = nullptr;
const VnlcPrimitiveType* VnlcPrimitiveType::STRING_TYPE = nullptr;

const VnlcPrimitiveType* VnlcPrimitiveType::byteType() {
    if (BYTE_TYPE == nullptr) {
        BYTE_TYPE = new VnlcPrimitiveType(VnlcPrimitiveTypeKind::BYTE);
    }
    return BYTE_TYPE;
}

const VnlcPrimitiveType* VnlcPrimitiveType::shortType() {
    if (SHORT_TYPE == nullptr) {
        SHORT_TYPE = new VnlcPrimitiveType(VnlcPrimitiveTypeKind::SHORT);
    }
    return SHORT_TYPE;
}

const VnlcPrimitiveType* VnlcPrimitiveType::intType() {
    if (INT_TYPE == nullptr) {
        INT_TYPE = new VnlcPrimitiveType(VnlcPrimitiveTypeKind::INT);
    }
    return INT_TYPE;
}

const VnlcPrimitiveType* VnlcPrimitiveType::longType() {
    if (LONG_TYPE == nullptr) {
        LONG_TYPE = new VnlcPrimitiveType(VnlcPrimitiveTypeKind::LONG);
    }
    return LONG_TYPE;
}

const VnlcPrimitiveType* VnlcPrimitiveType::floatType() {
    if (FLOAT_TYPE == nullptr) {
        FLOAT_TYPE = new VnlcPrimitiveType(VnlcPrimitiveTypeKind::FLOAT);
    }
    return FLOAT_TYPE;
}

const VnlcPrimitiveType* VnlcPrimitiveType::doubleType() {
    if (DOUBLE_TYPE == nullptr) {
        DOUBLE_TYPE = new VnlcPrimitiveType(VnlcPrimitiveTypeKind::DOUBLE);
    }
    return DOUBLE_TYPE;
}

const VnlcPrimitiveType* VnlcPrimitiveType::booleanType() {
    if (BOOLEAN_TYPE == nullptr) {
        BOOLEAN_TYPE = new VnlcPrimitiveType(VnlcPrimitiveTypeKind::BOOLEAN);
    }
    return BOOLEAN_TYPE;
}

const VnlcPrimitiveType* VnlcPrimitiveType::stringType() {
    if (STRING_TYPE == nullptr) {
        STRING_TYPE = new VnlcPrimitiveType(VnlcPrimitiveTypeKind::STRING);
    }
    return STRING_TYPE;
}
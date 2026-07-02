#ifndef VNLC_VOID_TYPE_HPP
#define VNLC_VOID_TYPE_HPP

#include "VnlcSemanticType.hpp"

class VnlcVoidType : public VnlcSemanticType {
public:
    VnlcVoidType(bool readonly);
};

#endif // VNLC_VOID_TYPE_HPP
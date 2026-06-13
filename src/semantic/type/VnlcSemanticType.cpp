#include "VnlcSemanticType.hpp"

VnlcSemanticType::VnlcSemanticType(bool readonly) : readonly(readonly) {}

bool VnlcSemanticType::isReadonly() const noexcept {
    return readonly;
}
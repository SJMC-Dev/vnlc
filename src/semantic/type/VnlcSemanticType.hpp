#ifndef VNLC_SEMANTIC_TYPE_HPP
#define VNLC_SEMANTIC_TYPE_HPP

#include <string_view>

class VnlcSemanticType {
protected:
    VnlcSemanticType();

public:
    [[nodiscard]] virtual std::string_view getFullTypeName() const noexcept = 0;

    virtual ~VnlcSemanticType() = default;
};

#endif // VNLC_SEMANTIC_TYPE_HPP
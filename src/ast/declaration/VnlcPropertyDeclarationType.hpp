#ifndef VNLC_PROPERTY_DECLARATION_TYPE_HPP
#define VNLC_PROPERTY_DECLARATION_TYPE_HPP

namespace VnlcPropertyDeclarationType {
    enum class AccessModifier {
        PUBLIC,
        PROTECTED,
        PRIVATE,
    };

    enum class Binding {
        INSTANCE,
        STATIC,
    };
}; // namespace VnlcPropertyDeclarationType

#endif // VNLC_PROPERTY_DECLARATION_TYPE_HPP
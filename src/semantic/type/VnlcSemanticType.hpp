#ifndef VNLC_SEMANTIC_TYPE_HPP
#define VNLC_SEMANTIC_TYPE_HPP

class VnlcSemanticType {
private:
    bool readonly;

protected:
    VnlcSemanticType(bool readonly);

public:
    [[nodiscard]] bool isReadonly() const noexcept;
};

#endif // VNLC_SEMANTIC_TYPE_HPP
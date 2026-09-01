#ifndef VNLC_IMPORTED_ITEM_HPP
#define VNLC_IMPORTED_ITEM_HPP

#include <string>
#include <string_view>

class VnlcImportedItem {
private:
    std::string name;

protected:
    VnlcImportedItem(std::string_view name);

public:
    [[nodiscard]] std::string_view getName() const;

    virtual ~VnlcImportedItem() = default;
};

#endif // VNLC_IMPORTED_ITEM_HPP
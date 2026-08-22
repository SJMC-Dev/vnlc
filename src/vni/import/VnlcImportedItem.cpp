#include "VnlcImportedItem.hpp"

VnlcImportedItem::VnlcImportedItem(std::string_view name) : name(name) {}

std::string_view VnlcImportedItem::getName() const {
    return name;
}
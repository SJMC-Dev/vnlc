#pragma once

#ifndef VNLC_DECLARATION_ITEM_HPP
#define VNLC_DECLARATION_ITEM_HPP

#include <string>
#include <optional>

namespace VnlcDeclarationItem {
    struct MetadataTerm {
        std::string key;
        std::optional<std::string> value; // if value is not provided, it is considered as "true"
    };
}

#endif // VNLC_DECLARATION_ITEM_HPP
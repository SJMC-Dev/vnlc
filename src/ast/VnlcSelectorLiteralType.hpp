#pragma once

#ifndef VNLC_SELECTOR_LITERAL_TYPE_HPP
#define VNLC_SELECTOR_LITERAL_TYPE_HPP

enum class VnlcSelectorLiteralType {
    NEAREST_PLAYER,   // @p
    RANDOM_PLAYER,    // @r
    ALL_PLAYERS,      // @a
    ALL_ENTITIES,     // @e
    CURRENT_EXECUTOR, // @s
    NEAREST_ENTITY,   // @n
};

#endif // VNLC_SELECTOR_LITERAL_TYPE_HPP
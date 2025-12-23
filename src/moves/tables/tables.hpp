#pragma once

#include <cstdint>
#include <vector>


namespace ChessTables {
    extern uint64_t knight_attacks[64];
    extern uint64_t king_attacks[64];

    void init_knight_table();
    void init_king_table();
}
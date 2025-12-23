#pragma once

#include "game/game_constants.hpp"

#include <cstdint>
#include <vector>


namespace ChessTables {
    extern uint64_t knight_attacks[64];
    extern uint64_t king_attacks[64];
    extern uint64_t pawn_pushes[2][64];
    extern uint64_t pawn_attacks[2][64];

    void init_knight_table();
    void init_king_table();
    void init_white_pawn_pushes_table();
    void init_black_pawn_pushes_table();
    void init_white_pawn_attacks_table();
    void init_black_pawn_attacks_table();
}
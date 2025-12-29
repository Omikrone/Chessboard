#pragma once

#include "game/game_constants.hpp"

#include <cstdint>
#include <vector>


/**
 * @brief namespace for the piece's pre-computed moves tables
 */
namespace ChessTables {
    extern uint64_t knight_attacks[64];
    extern uint64_t king_attacks[64];
    extern uint64_t pawn_pushes[2][64];
    extern uint64_t pawn_attacks[2][64];

    /**
     * @brief Computes the knight moves in it's table.
     */
    void init_knight_table();

    /**
     * @brief Computes the king moves in it's table.
     */
    void init_king_table();

    /**
     * @brief Computes the white pawn pushes in it's table.
     */
    void init_white_pawn_pushes_table();

    /**
     * @brief Computes the black pawn pushes in it's table.
     */
    void init_black_pawn_pushes_table();

    /**
     * @brief Computes the white pawns attacks in it's table.
     */
    void init_white_pawn_attacks_table();

    /**
     * @brief Computes the black pawns attacks in it's table.
     */
    void init_black_pawn_attacks_table();
}
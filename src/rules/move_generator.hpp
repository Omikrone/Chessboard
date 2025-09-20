// move_generator.hpp

#pragma once

#include "board/board.hpp"
#include "game_constants.hpp"
#include "components/move.hpp"

#include <cstdint>
#include <vector>


/**
 * @brief Static class respresenting the generation of raw moves.
 *
 * Manages the generator of the differents moves by the pieces.
 */
class MoveGenerator
{
    public:
        static std::vector<Move> pawn_moves(const uint8_t square, const Color side, const uint64_t empty_squares, const uint64_t opponent_side);
        static std::vector<Move> knight_moves(const uint8_t square);
        static std::vector<Move> bishop_moves(const uint8_t square, const uint64_t occupancy);
        static std::vector<Move> rook_moves(const uint8_t square, const uint64_t occupancy);
        static std::vector<Move> queen_moves(const uint8_t square, const uint64_t occupancy);
        static std::vector<Move> king_moves(const uint8_t square);
};
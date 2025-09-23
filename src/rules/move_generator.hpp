// move_generator.hpp

#pragma once

#include "game_constants.hpp"
#include "components/move.hpp"
#include "bitboards.hpp"

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
        static std::vector<Move> all_possible_moves(const Color side, const GameState& game_state, const Bitboards& board);
        static std::vector<Move> piece_moves(const int square, const Color side, const PieceType& piece_type, const GameState& game_state);
        static std::vector<Move> pawn_moves(const int square, const Color side, const uint64_t empty_squares, const uint64_t opponent_side);
        static std::vector<Move> knight_moves(const int square, const uint64_t side, const uint64_t opponent_side);
        static std::vector<Move> bishop_moves(const int square, const uint64_t side, const uint64_t opponent_side);
        static std::vector<Move> rook_moves(const int square, const uint64_t side, const uint64_t opponent_side);
        static std::vector<Move> queen_moves(const int square, const uint64_t side, const uint64_t opponent_side);
        static std::vector<Move> king_moves(const int square, const uint64_t side, const uint64_t opponent_side, const uint64_t rooks, int castling_rights);
};
// move_generator.hpp

#pragma once

#include <cstdint>
#include <vector>

#include "board/bitboards.hpp"
#include "game/game_constants.hpp"
#include "moves/move.hpp"

/**
 * @brief Class respresenting the generation of raw moves.
 *
 * Manages the generator of the differents moves by the pieces.
 */
class MoveGenerator {
   private:
      const Position& _pos;
      const Bitboards& _board;
      MoveGenerator(const Position& pos, const Bitboards& board);
      ~MoveGenerator() = default;

   public:
      std::vector<Move> all_possible_moves(const Color side);
      std::vector<Move> piece_moves(const int square, const Color side, const PieceType &piece_type);
      std::vector<Move> pawn_moves(const int square, const Color side);
      std::vector<Move> knight_moves(const int square, const Color side);
      std::vector<Move> bishop_moves(const int square, const Color side);
      std::vector<Move> rook_moves(const int square, const Color side);
      std::vector<Move> queen_moves(const int square, const Color side);
      std::vector<Move> king_moves(const int square, const Color side);
};
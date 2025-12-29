#pragma once

#include <cstdint>
#include <vector>

#include "board/bitboards.hpp"
#include "game/game_constants.hpp"
#include "moves/move.hpp"
#include "moves/tables/tables.hpp"
#include "bit_scan.hpp"

/**
 * @brief Class representing the generation of raw moves.
 *
 * Manages the generator of the differents moves by the pieces.
 */
class MoveGenerator {
   private:
      const Position& _pos;
      const Bitboards& _board;

   public:
      MoveGenerator(const Position& pos, const Bitboards& board);
      ~MoveGenerator() = default;

      /**
       * @brief Generates all possible moves for a side of the chessboard.
       */
      std::vector<Move> all_possible_moves(const Color side);

      /**
       * @brief Generates all possible moves for a piece on a given square.
       */
      std::vector<Move> piece_moves(const int square, const Color side, const PieceType &piece_type);

      /**
       * @brief Generates all possible pawn moves from a given square.
       */
      std::vector<Move> pawn_moves(const int square, const Color side);

      /**
       * @brief Generates all possible knight moves from a given square.
       */
      std::vector<Move> knight_moves(const int square, const Color side);

      /**
       * @brief Generates all possible bishop moves from a given square.
       */
      std::vector<Move> bishop_moves(const int square, const Color side);

      /**
       * @brief Generates all possible rook moves from a given square.
       */
      std::vector<Move> rook_moves(const int square, const Color side);

      /**
       * @brief Generates all possible queen moves from a given square.
       */
      std::vector<Move> queen_moves(const int square, const Color side);

      /**
       * @brief Generates all possible king moves from a given square.
       */
      std::vector<Move> king_moves(const int square, const Color side);
};
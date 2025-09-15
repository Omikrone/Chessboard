// move_generator.hpp

#pragma once

#include "game/board/board.hpp"
#include "game/components/move.hpp"


/**
 * @brief Static class respresenting the generation of raw moves.
 *
 * Manages the generator of the differents moves by the pieces.
 */
class MoveGenerator
{
    public:

        /**
         * @brief Generates all the possible raw moves for a player.
         *
         * @param board The game board on which the moves will be generated.
         * @param side The color of the pieces to generate the moves.
         * @return A vector of all the possible moves.
         */
        static std::vector<Move> get_all_possible_moves(GameBoard& board, Color side);

        /**
         * @brief Generates the raw moves for a given piece.
         *
         * @param board The game board on which the moves will be generated.
         * @param piece The piece to generate the moves.
         * @return A vector of the possible moves.
         */
        static std::vector<Move> get_possible_moves(GameBoard& board, Piece *piece);
};

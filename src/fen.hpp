// fen.hpp

#pragma once

#include "bitboards.hpp"
#include "game_constants.hpp"

#include <cctype>
#include <string>


/**
 * @brief Static class to respresent a game in the FEN notation.
 *
 * Manages the conversion of the state of a game to the FEN notation and vice versa.
 */
class FEN
{
    public:

        /**
         * @brief Converts the state of a game into a FEN notation.
         *
         * @param game Game to convert.
         * @return A string respresenting the FEN notation.
         */
        static std::string to_string(const GameState& game, const Bitboards& board);

        /**
         * @brief Gets the FEN notation of a piece.
         *
         * @param piece_color Color of the piece.
         * @param piece_type Type of the piece.
         * @return A char representing the piece into a FEN notation.
         */
        static char piece_symbol(const Color piece_color, const PieceType piece_type);

        /**
         * @brief Gets the FEN notation of the castling rights.
         *
         * @param rights Binary notation of the rights (1 for right of castle, else 0).
         * @return A string representing the castling rights of both sides in the game.
         */
        static std::string castling_rights(int rights);
};
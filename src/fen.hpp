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
        static std::string to_string(GameState& game, Bitboards& board);

        static char piece_symbol(const Color piece_color, const PieceType piece_type);

        static std::string castling_rights(int rights);
};
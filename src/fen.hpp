// fen.hpp

#pragma once

#include "game/game.hpp"

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
        static std::string to_string(Game& game);
};
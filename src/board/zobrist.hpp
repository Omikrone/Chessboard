// zobrist.hpp

#pragma once

#include "bitboards.hpp"
#include <random>
#include <ctime>

/**
 * @brief Class representing the Zobrist hashing of a chess position.
 *
 * Manages the generation and calculation of the Zobrist hash for a given position.
 */
class Zobrist
{
    private:
        uint64_t _table[64][12];
        uint64_t _turn;
        Position& _position;
        Bitboards& _board;
        
    public:
        Zobrist(Position& game_state, Bitboards& board);
        ~Zobrist() = default;

        /**
         * @brief Calculates the Zobrist hash for the current position.
         *
         * @return The Zobrist hash as a 64-bit unsigned integer.
         */
        const uint64_t hash();
};

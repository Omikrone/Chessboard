// zobrist.hpp

#pragma once

#include "bitboards.hpp"
#include <random>
#include <ctime>

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
        const uint64_t hash();
};

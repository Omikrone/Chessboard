// zobrist.hpp

#pragma once

#include "bitboards.hpp"
#include <random>

class Zobrist
{
    private:
        int _table[64][12];
    public:
        Zobrist();
        ~Zobrist() = default;

        const int hash(GameState& game_state, Bitboards& board);
};

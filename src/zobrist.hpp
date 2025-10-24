// zobrist.hpp

#pragma once

#include "bitboards.hpp"
#include <random>

class Zobrist
{
    private:
        int _table[64][12];
        GameState& _game_state;
        Bitboards& _board;
        
    public:
        Zobrist(GameState& game_state, Bitboards& board);
        ~Zobrist() = default;

        const int hash();
};

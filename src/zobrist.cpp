// zobrist.cpp

#include "zobrist.hpp"


Zobrist::Zobrist()
{
    std::srand(std::time({}));
    
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; i < 12; i++)
        {
            _table[i][j] = std::rand();
        } 
    }
}

const int Zobrist::hash(GameState& game_state, Bitboards& board) {
    int hash = 0;

    if (game_state.side_to_move == Color::BLACK) hash ^= game_state.side_to_move;
    for (int i = 0; i < 64; i++)
    {
        Color side = board.is_occupied(i);
        if (side != Color::NONE) {
            hash ^= board.get_piece_type(side, i);
        }
    }
    return hash;
}

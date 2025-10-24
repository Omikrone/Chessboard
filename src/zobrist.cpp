// zobrist.cpp

#include "zobrist.hpp"


Zobrist::Zobrist(GameState& game_state, Bitboards& board):
_game_state(game_state),
_board(board)
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

const int Zobrist::hash() {
    int hash = 0;

    if (_game_state.side_to_move == Color::BLACK) hash ^= _game_state.side_to_move;
    for (int i = 0; i < 64; i++)
    {
        Color side = _board.is_occupied(i);
        if (side != Color::NONE) {
            hash ^= _board.get_piece_type(side, i);
        }
    }
    return hash;
}

//move_validator.cpp

#include "rules/move_validator.hpp"


MoveValidator::MoveValidator(const GameState& game_state, const Bitboards& board, MoveExecutor& executor) 
: _game_state(game_state), _board(board), _executor(executor) {}


bool MoveValidator::is_square_attacked(const int square, const Color opponent) const {
    std::vector<Move> enemy_moves = MoveGenerator::all_possible_moves(opponent, _game_state, _board);
    for (Move& m: enemy_moves) {
        if (m.to == square) return true;
    }
    return false;
}

bool MoveValidator::is_king_in_check(const Color king_color) const {
    int square = -1;
    for (int i = 0; i < 64; i++) {
        if (_game_state.pieces[king_color][PieceType::KING] & (1ULL << i)) {
            square = i;
            break;
        }
    }
    if (king_color == Color::WHITE) return is_square_attacked(square, Color::BLACK);
    else return is_square_attacked(square, Color::WHITE);
}


bool MoveValidator::is_legal(const Move& move) {

    _executor.make_move(_game_state.side_to_move, move);
    return is_king_in_check(_game_state.side_to_move);
}
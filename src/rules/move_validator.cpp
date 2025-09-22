//move_validator.cpp

#include "rules/move_validator.hpp"


MoveValidator::MoveValidator(const GameState& game_state, const Bitboards& board) : _game_state(game_state), _board(board) {}


bool MoveValidator::is_square_attacked(const uint8_t square, const Color opponent) const {
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


bool MoveValidator::is_legal(const Move& move) const {
    Bitboards clone_board = _board;

    clone_board.
}
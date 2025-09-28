//move_validator.cpp

#include "rules/move_validator.hpp"
#include <iostream>
#include "fen.hpp"


MoveValidator::MoveValidator(GameState& game_state, Bitboards& board, MoveExecutor& executor) 
: _game_state(game_state), _board(board), _executor(executor) {}


bool MoveValidator::is_square_attacked(const int square, const Color opponent) const {
    std::vector<Move> enemy_moves = MoveGenerator::all_possible_moves(opponent, _game_state, _board);
    for (Move& m: enemy_moves) {
        if (m.to == square) return true;
    }
    return false;
}

int MoveValidator::find_king(const Color king_color) const {
    int square = -1;
    for (int i = 0; i < 64; i++) {
        if (_game_state.pieces[king_color][PieceType::KING] & (1ULL << i)) {
            square = i;
            break;
        }
    }
    return square;
}


bool MoveValidator::is_king_in_check(const Color king_color) const {
    int square = find_king(king_color);
    if (king_color == Color::WHITE) return is_square_attacked(square, Color::BLACK);
    else return is_square_attacked(square, Color::WHITE);
}


bool MoveValidator::is_legal(const Move& move) {

    bool result;
    std::cout << "BEFORE MAKE : " << std::endl;
    _board.print_board(_game_state.colors[_game_state.side_to_move]);

    if (move.type == MoveType::CASTLE_KINGSIDE || move.type==MoveType::CASTLE_QUEENSIDE) {
        return check_castle(move, _game_state.side_to_move);
    }

    _executor.make_move(_game_state.side_to_move, move);
    std::cout << FEN::to_string(_game_state, _board) << std::endl;
    result = !is_king_in_check(_game_state.side_to_move);
    if (result) std::cout << _game_state.side_to_move << std::endl;
    _executor.unmake_last_move();
    std::cout << "AFTER UNMAKE : " << std::endl;
    _board.print_board(_game_state.colors[_game_state.side_to_move]);
    return result;
}


bool MoveValidator::check_castle(const Move& move, const Color king_color) const {
    int square = find_king(king_color);
    Color opponent_color = (king_color == Color::WHITE) ? Color::BLACK : Color::WHITE;

    if (move.type == MoveType::CASTLE_KINGSIDE && (is_square_attacked(square, opponent_color) 
    || is_square_attacked(square + 1, opponent_color) 
    || is_square_attacked(square + 2, opponent_color))) {
        return false;
    }

    if (move.type == MoveType::CASTLE_QUEENSIDE && (is_square_attacked(square, opponent_color) 
    || is_square_attacked(square - 1, opponent_color) 
    || is_square_attacked(square - 2, opponent_color))) {
        return false;
    }

    return true;
}
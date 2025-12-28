// move_validator.cpp

#include "rules/move_validator.hpp"

MoveValidator::MoveValidator(Position &game_state, Bitboards &board, MoveExecutor &executor, MoveGenerator& generator)
    : _position(game_state), _board(board), _executor(executor), _generator(generator) {}

bool MoveValidator::is_square_attacked(const int square, const Color opponent) const {
    std::vector<Move> enemy_moves = _generator.all_possible_moves(opponent);
    for (Move &m : enemy_moves) {
        if (m.to == square) return true;
    }
    return false;
}

int MoveValidator::find_king(const Color king_color) const {
    int square = -1;
    for (int i = 0; i < 64; i++) {
        if (_position.pieces[king_color][PieceType::KING] & (1ULL << i)) {
            square = i;
            break;
        }
    }
    return square;
}

bool MoveValidator::is_king_in_check(const Color king_color) const {
    int square = find_king(king_color);
    if (king_color == Color::WHITE)
        return is_square_attacked(square, Color::BLACK);
    else
        return is_square_attacked(square, Color::WHITE);
}

bool MoveValidator::is_legal(const Move &move) {
    bool result;

    if (move.type == MoveType::CASTLE_KINGSIDE || move.type == MoveType::CASTLE_QUEENSIDE) {
        return check_castle(move, _position.side_to_move);
    }

    _executor.make_move(_position.side_to_move, move);
    result = !is_king_in_check(_position.side_to_move);
    _executor.unmake_last_move();
    return result;
}

std::pair<bool, bool> MoveValidator::is_legal_and_check(const Move &move) {
    bool legal = false;
    bool gives_check = false;
    
    if (move.type == MoveType::CASTLE_KINGSIDE || move.type == MoveType::CASTLE_QUEENSIDE) {
        legal = check_castle(move, _position.side_to_move);
        if (legal) {
            _executor.make_move(_position.side_to_move, move);
            gives_check = is_king_in_check((_position.side_to_move == Color::WHITE) ? Color::BLACK : Color::WHITE);
            _executor.unmake_last_move();
        }
        return {legal, gives_check};
    }
    
    _executor.make_move(_position.side_to_move, move);
    legal = !is_king_in_check(_position.side_to_move);
    gives_check = is_king_in_check((_position.side_to_move == Color::WHITE) ? Color::BLACK : Color::WHITE);
    
    _executor.unmake_last_move();
    return {legal, gives_check};
}

bool MoveValidator::check_castle(const Move &move, const Color king_color) const {
    int square = find_king(king_color);
    Color opponent_color = (king_color == Color::WHITE) ? Color::BLACK : Color::WHITE;

    if (move.type == MoveType::CASTLE_KINGSIDE &&
        (is_square_attacked(square, opponent_color) || is_square_attacked(square + 1, opponent_color) ||
         is_square_attacked(square + 2, opponent_color))) {
        return false;
    }

    if (move.type == MoveType::CASTLE_QUEENSIDE &&
        (is_square_attacked(square, opponent_color) || is_square_attacked(square - 1, opponent_color) ||
         is_square_attacked(square - 2, opponent_color))) {
        return false;
    }

    return true;
}
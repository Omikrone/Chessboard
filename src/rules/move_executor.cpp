// move_executor.hpp

#include "move_executor.hpp"


MoveExecutor::MoveExecutor(GameHistory& history, GameState& state, Bitboards& board):
    _history(history),
    _game_state(state),
    _board(board)
    {}


void MoveExecutor::make_move(const Color side, const Move& move) {
    switch (move.type)
    {
        case MoveType::CASTLE_KINGSIDE:
            castle_kingside(side, move);
            break;
        case MoveType::CASTLE_QUEENSIDE:
            castle_queenside(side, move);
            break;
        case MoveType::EN_PASSANT:
            en_passant(side, move);
            break;
        case MoveType::PROMOTION:
            normal(side, move);
            promotion(side, move.from, PieceType::QUEEN);
            break;
        default:
            normal(side, move);
            break;
    }
    _history.push(_game_state);
}


void MoveExecutor::unmake_last_move() {
    GameState last_state = _history.last();
    _game_state = last_state;
    _history.pop();
}



void MoveExecutor::normal(const Color side, const Move& move) {
    if (move.take)
    {
        Color opponent_color = (side == Color::WHITE ? Color::BLACK : Color::WHITE);
        PieceType opponent_piece = _board.get_piece_type(opponent_color, move.to);
        _board.remove_piece(opponent_color, opponent_piece, move.to);
    }
    PieceType piece_type = _board.get_piece_type(side, move.from);
    _board.move_piece(side, piece_type, move.from, move.to);
}


void MoveExecutor::castle_kingside(const Color side, const Move& move) {
    _board.move_piece(side, PieceType::KING, move.from, move.to);
    _board.move_piece(side, PieceType::ROOK, move.from + 3, move.from + 1);
}


void MoveExecutor::castle_queenside(const Color side, const Move& move) {
    _board.move_piece(side, PieceType::KING, move.from, move.to);
    _board.move_piece(side, PieceType::ROOK, move.from - 4, move.from - 1);
}


void MoveExecutor::en_passant(const Color side, const Move& move) {
    _board.move_piece(side, PieceType::PAWN, move.from, move.to);
    Color opponent_color = (side == Color::WHITE ? Color::BLACK : Color::WHITE);
    _board.remove_piece(opponent_color, PieceType::PAWN, move.to - 8);
}


void MoveExecutor::promotion(const Color side, const int square, const PieceType new_piece) {
    _board.remove_piece(side, PieceType::PAWN, square);
    _board.add_piece(side, new_piece, square);
}
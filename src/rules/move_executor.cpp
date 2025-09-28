// move_executor.hpp

#include "move_executor.hpp"
#include <iostream>


MoveExecutor::MoveExecutor(GameHistory& history, GameState& state, Bitboards& board):
    _history(history),
    _game_state(state),
    _board(board)
    {}


void MoveExecutor::make_move(const Color side, const Move& move) {
    move.print();
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
            promotion(side, move.to, PieceType::QUEEN);
            break;
        default:
            normal(side, move);
            break;
    }
    _history.push(_game_state);
    std::cout << _history.size() << std::endl;
}


void MoveExecutor::unmake_last_move() {
    _history.pop();
    GameState last_state = _history.last();
    std::cout << "LAST STATE : " << std::endl;
    _game_state = last_state;
    _board.print_board(last_state.colors[last_state.side_to_move]);
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
    if (piece_type == PieceType::KING || (piece_type == PieceType::ROOK && (move.from == 7 || move.from == 63))) {
        _game_state.castling_rights &= ~(1 << (side * 2 + 1));
    }
    if (piece_type == PieceType::KING || (piece_type == PieceType::ROOK && (move.from == 0 || move.from == 56))) {
        _game_state.castling_rights &= ~(1 << (side * 2 + 2));
    }
}


void MoveExecutor::castle_kingside(const Color side, const Move& move) {
    _board.move_piece(side, PieceType::KING, move.from, move.to);
    _board.move_piece(side, PieceType::ROOK, move.from + 3, move.from + 1);
    _game_state.castling_rights &= ~((side * 2 + 1) << 0);
    _game_state.castling_rights &= ~((side * 2 + 2) << 0);
}


void MoveExecutor::castle_queenside(const Color side, const Move& move) {
    _board.move_piece(side, PieceType::KING, move.from, move.to);
    _board.move_piece(side, PieceType::ROOK, move.from - 4, move.from - 1);
    _game_state.castling_rights &= ~((side * 2 + 1) << 0);
    _game_state.castling_rights &= ~((side * 2 + 2) << 0);
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
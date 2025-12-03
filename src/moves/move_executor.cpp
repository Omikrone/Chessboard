// move_executor.hpp

#include "move_executor.hpp"

MoveExecutor::MoveExecutor(GameHistory &history, Position &state, Bitboards &board, Zobrist &zobrist)
    : _history(history), _position(state), _board(board), _zobrist(zobrist) {}

void MoveExecutor::make_move(const Color side, const Move &move, const std::optional<PieceType> promotion_piece) {
    UndoMove undo;
    undo.castling_rights = _position.castling_rights;
    undo.fullmove_number = _position.fullmove_number;
    undo.en_passant_square = _position.en_passant_square;
    undo.halfmove_clock = _position.halfmove_clock;
    undo.side_to_move = _position.side_to_move;

    switch (move.type) {
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
            normal(undo, side, move);
            if (promotion_piece.has_value())
                promotion(side, move.to, promotion_piece.value());
            else
                promotion(side, move.to, PieceType::QUEEN);
            break;
        default:
            normal(undo, side, move);
            break;
    }
    PieceType piece_type = _board.get_piece_type(side, move.to);
    if (piece_type == PieceType::PAWN) {
        if (std::abs(move.from - move.to) == 16) {
            _position.en_passant_square = (move.from + move.to) / 2;
        } else {
            _position.en_passant_square = -1;
        }
    } else {
        _position.en_passant_square = -1;
    }
    if (piece_type == PieceType::PAWN || move.take)
        _position.halfmove_clock = 0;
    else
        _position.halfmove_clock++;

    undo.move = move;
    undo.zobrist_hash = _zobrist.hash();

    _position.fullmove_number++;

    _history.push(undo);
}

void MoveExecutor::unmake_last_move() {
    UndoMove undo = _history.pop();

    switch (undo.move.type) {
        case MoveType::CASTLE_KINGSIDE:
            undo_castle_kingside(undo.side_to_move, undo.move);
            break;
        case MoveType::CASTLE_QUEENSIDE:
            undo_castle_queenside(undo.side_to_move, undo.move);
            break;
        case MoveType::EN_PASSANT:
            undo_en_passant(undo.side_to_move, undo.move, undo.en_passant_square);
            break;
        case MoveType::PROMOTION:
            undo_promotion(undo.side_to_move, undo.move);
            undo_normal(undo.side_to_move, undo.move, undo.taken_piece);
            break;
        default:
            undo_normal(undo.side_to_move, undo.move, undo.taken_piece);
            break;
    }
    _position.castling_rights = undo.castling_rights;
    _position.en_passant_square = undo.en_passant_square;
    _position.fullmove_number = undo.fullmove_number;
    _position.halfmove_clock = undo.halfmove_clock;
    _position.side_to_move = undo.side_to_move;
    _position.zobrist_hash = undo.zobrist_hash;
}

void MoveExecutor::normal(UndoMove &undo, const Color side, const Move &move) {
    if (move.take) {
        Color opponent_color = (side == Color::WHITE ? Color::BLACK : Color::WHITE);
        PieceType opponent_piece = _board.get_piece_type(opponent_color, move.to);
        undo.taken_piece = opponent_piece;
        _board.remove_piece(opponent_color, opponent_piece, move.to);
    }
    PieceType piece_type = _board.get_piece_type(side, move.from);
    _board.move_piece(side, piece_type, move.from, move.to);
    if (piece_type == PieceType::KING || (piece_type == PieceType::ROOK && (move.from == 7 || move.from == 63))) {
        _position.castling_rights &= ~(1 << (side * 2));
    }
    if (piece_type == PieceType::KING || (piece_type == PieceType::ROOK && (move.from == 0 || move.from == 56))) {
        _position.castling_rights &= ~(1 << (side * 2 + 1));
    }
}

void MoveExecutor::undo_normal(const Color side, const Move &move, PieceType taken_piece) {
    PieceType piece_type = _board.get_piece_type(side, move.to);
    _board.move_piece(side, piece_type, move.to, move.from);
    if (move.take) {
        Color opponent_color = (side == Color::WHITE ? Color::BLACK : Color::WHITE);
        _board.add_piece(opponent_color, taken_piece, move.to);
    }
}

void MoveExecutor::castle_kingside(const Color side, const Move &move) {
    _board.move_piece(side, PieceType::KING, move.from, move.to);
    _board.move_piece(side, PieceType::ROOK, move.from + 3, move.from + 1);
    _position.castling_rights &= ~((1 << (side * 2)) | (1 << (side * 2 + 1)));
}

void MoveExecutor::undo_castle_kingside(const Color side, const Move &move) {
    _board.move_piece(side, PieceType::KING, move.to, move.from);
    _board.move_piece(side, PieceType::ROOK, move.from + 1, move.from + 3);
}

void MoveExecutor::castle_queenside(const Color side, const Move &move) {
    _board.move_piece(side, PieceType::KING, move.from, move.to);
    _board.move_piece(side, PieceType::ROOK, move.from - 4, move.from - 1);
    _position.castling_rights &= ~((1 << (side * 2)) | (1 << (side * 2 + 1)));
}

void MoveExecutor::undo_castle_queenside(const Color side, const Move &move) {
    _board.move_piece(side, PieceType::KING, move.to, move.from);
    _board.move_piece(side, PieceType::ROOK, move.from - 1, move.from - 4);
}

void MoveExecutor::en_passant(const Color side, const Move &move) {
    _board.move_piece(side, PieceType::PAWN, move.from, move.to);
    Color opponent_color = (side == Color::WHITE ? Color::BLACK : Color::WHITE);
    int en_passant_square = move.to;
    if (side == Color::WHITE) {
        _board.remove_piece(opponent_color, PieceType::PAWN, en_passant_square - 8);
    } else {
        _board.remove_piece(opponent_color, PieceType::PAWN, en_passant_square + 8);
    }
}

void MoveExecutor::undo_en_passant(const Color side, const Move &move, int en_passant_square) {
    _board.move_piece(side, PieceType::PAWN, move.to, move.from);
    Color opponent_color = (side == Color::WHITE ? Color::BLACK : Color::WHITE);
    if (side == Color::WHITE)
        _board.add_piece(opponent_color, PieceType::PAWN, en_passant_square - 8);
    else
        _board.add_piece(opponent_color, PieceType::PAWN, en_passant_square + 8);
}

void MoveExecutor::promotion(const Color side, const int square, const PieceType new_piece) {
    _board.remove_piece(side, PieceType::PAWN, square);
    _board.add_piece(side, new_piece, square);
}

void MoveExecutor::undo_promotion(const Color side, const Move &move) {
    PieceType promoted_piece = _board.get_piece_type(side, move.to);
    _board.remove_piece(side, promoted_piece, move.to);
    _board.add_piece(side, PieceType::PAWN, move.to);
}
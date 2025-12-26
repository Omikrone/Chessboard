// move_generator.cpp

#include "moves/move_generator.hpp"

MoveGenerator::MoveGenerator(const Position& pos, const Bitboards& board):
    _pos(pos),
    _board(board)
{
    ChessTables::init_knight_table();
    ChessTables::init_king_table();
    ChessTables::init_white_pawn_pushes_table();
    ChessTables::init_white_pawn_attacks_table();
    ChessTables::init_black_pawn_pushes_table();
    ChessTables::init_black_pawn_attacks_table();
}

std::vector<Move> MoveGenerator::all_possible_moves(const Color side) {
    std::vector<Move> moves;
    uint64_t mask;
    for (int i = 0; i < 64; i++) {
        mask = 1ULL << i;

        if ((_pos.colors[side] & mask) == 0) continue;

        const PieceType piece_type = _board.get_piece_type(side, i);
        if (piece_type == PieceType::NONE_PIECE) {
            continue;
        }

        std::vector<Move> p_moves = piece_moves(i, side, piece_type);
        moves.insert(moves.end(), p_moves.begin(), p_moves.end());
    }
    return moves;
}

std::vector<Move> MoveGenerator::piece_moves(const int square, const Color side, const PieceType &piece_type) {
    std::vector<Move> moves;

    switch (piece_type) {
        case PieceType::PAWN:
            moves = pawn_moves(square, side);
            break;
        case PieceType::BISHOP:
            moves = bishop_moves(square, side);
            break;
        case PieceType::KNIGHT:
            moves = knight_moves(square, side);
            break;
        case PieceType::ROOK:
            moves = rook_moves(square, side);
            break;
        case PieceType::QUEEN:
            moves = queen_moves(square, side);
            break;
        case PieceType::KING:
            moves = king_moves(square, side);
            break;
        default:
            break;
    }
    return moves;
}

std::vector<Move> MoveGenerator::pawn_moves(const int square, const Color side) {
    std::vector<Move> moves;
    Color opponent = (side == Color::WHITE) ? Color::BLACK : Color::WHITE;
    uint64_t empty_squares = ~_pos.all_pieces;

    int direction = (side == Color::WHITE) ? 8 : -8;
    int start_rank = (side == Color::WHITE) ? 1 : 6;
    int promotion_rank = (side == Color::WHITE) ? 7 : 0;
    int rank = square / 8;

    int to = square + direction;
    if ((empty_squares >> to) & 1) {
        bool is_promo = (to / 8 == promotion_rank);
        MoveType type = is_promo ? MoveType::PROMOTION : MoveType::NORMAL;
        
        moves.push_back({square, to, type, false});
        if (rank == start_rank) {
            int to_double = square + (direction * 2);
            if ((empty_squares >> to_double) & 1) {
                moves.push_back({square, to_double, MoveType::NORMAL, false});
            }
        }
    }

    uint64_t potential_attacks = ChessTables::pawn_attacks[side][square];
    uint64_t regular_captures = potential_attacks & _pos.colors[opponent];
    
    while (regular_captures) {
        int to_attack = bitscan_forward(regular_captures);
        bool is_promo = (to_attack / 8 == promotion_rank);
        MoveType type = is_promo ? MoveType::PROMOTION : MoveType::NORMAL;

        moves.push_back({square, to_attack, type, true});
        regular_captures &= regular_captures - 1;
    }


    if (_pos.en_passant_square != -1) {
        uint64_t ep_mask = 1ULL << _pos.en_passant_square;

        if (potential_attacks & ep_mask) {
            moves.push_back({square, _pos.en_passant_square, MoveType::EN_PASSANT, true});
        }
    }

    return moves;
}

std::vector<Move> MoveGenerator::knight_moves(const int square, const Color side) {
    std::vector<Move> moves;
    moves.reserve(8);
    Color opponent = (side == Color::WHITE) ? Color::BLACK : Color::WHITE;
    
    uint64_t attacks = ChessTables::knight_attacks[square];

    attacks &= ~_pos.colors[side];
    
    while (attacks) {
        int to = bitscan_forward(attacks);
        bool is_capture = (_pos.colors[opponent] >> to) & 1;
        moves.push_back({square, to, MoveType::NORMAL, is_capture});
        attacks &= attacks - 1;
    }
    
    return moves;
}

std::vector<Move> MoveGenerator::rook_moves(const int square, const Color side) {
    std::vector<Move> moves;
    Color opponent = (side == Color::WHITE) ? Color::BLACK : Color::WHITE;
    
    const int directions[] = {8, 1, -8, -1};
    int fromRow = square / 8;
    int fromCol = square % 8;
    
    for (int d : directions) {
        for (int count = 1; ; count++) {
            int to = square + d * count;
            if (to < 0 || to > 63) break;

            if (d == 1 || d == -1) {
                int toRow = to / 8;
                if (toRow != fromRow) break;
            }

            if (d == 8 || d == -8) {
                int toCol = to % 8;
                if (toCol != fromCol) break;
            }
            
            uint64_t mask = 1ULL << to;
            
            if (_pos.colors[side] & mask) {
                break;
            } else if (_pos.colors[opponent] & mask) {
                moves.push_back({square, to, MoveType::NORMAL, true});
                break;
            } else {
                moves.push_back({square, to, MoveType::NORMAL, false});
            }
        }
    }
    return moves;
}

std::vector<Move> MoveGenerator::bishop_moves(const int square, const Color side) {
    std::vector<Move> moves;
    Color opponent = (side == Color::WHITE) ? Color::BLACK : Color::WHITE;

    const int directions[] = {7, -7, -9, 9};
    int fromX = square % 8;
    int fromY = square / 8;

    int count;
    for (int d : directions) {
        count = 1;
        while (true) {
            int to = square + d * count;

            int toX = to % 8;
            int toY = to / 8;
            if (to < 0 || to > 63) break;

            if (std::abs(toX - fromX) != count || std::abs(toY - fromY) != count) break;

            uint64_t mask = 1ULL << to;

            if (_pos.colors[side] & mask)
                break;
            else if (_pos.colors[opponent] & mask) {
                moves.push_back({square, to, MoveType::NORMAL, true});
                break;
            } else {
                moves.push_back({square, to, MoveType::NORMAL, false});
                count++;
            }
        }
    }
    return moves;
}

std::vector<Move> MoveGenerator::queen_moves(const int square, const Color side) {
    std::vector<Move> bishop_moves = MoveGenerator::bishop_moves(square, side);
    std::vector<Move> rook_moves = MoveGenerator::rook_moves(square, side);
    bishop_moves.insert(bishop_moves.end(), rook_moves.begin(), rook_moves.end());
    return bishop_moves;
}

std::vector<Move> MoveGenerator::king_moves(const int square, const Color side) {
    std::vector<Move> moves;
    Color opponent = (side == Color::WHITE) ? Color::BLACK : Color::WHITE;

    uint64_t attacks = ChessTables::king_attacks[square];

    attacks &= ~_pos.colors[side];
    
    while (attacks) {
        int to = bitscan_forward(attacks);
        bool is_capture = (_pos.colors[opponent] >> to) & 1;
        moves.push_back({square, to, MoveType::NORMAL, is_capture});
        attacks &= attacks - 1;
    }

    uint64_t all_pieces = _pos.colors[side] | _pos.colors[opponent];
    if ((_pos.castling_rights >> (side * 2) & 1) && ((all_pieces >> (square + 1)) & 1) == 0 &&
        ((all_pieces >> (square + 2)) & 1) == 0) {
        moves.push_back({square, square + 2, MoveType::CASTLE_KINGSIDE, false});
    }
    if ((_pos.castling_rights >> (side * 2 + 1) & 1) && ((all_pieces >> (square - 1)) & 1) == 0 &&
        ((all_pieces >> (square - 2)) & 1) == 0 && ((all_pieces >> (square - 3)) & 1) == 0) {
        moves.push_back({square, square - 2, MoveType::CASTLE_QUEENSIDE, false});
    }

    return moves;
}
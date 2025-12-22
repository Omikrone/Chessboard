// move_generator.cpp

#include "moves/move_generator.hpp"

MoveGenerator::MoveGenerator(const Position& pos, const Bitboards& board):
    _pos(pos),
    _board(board)
{
    ChessTables::init_knight_table();
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

    int rank = square / 8;
    int file = square % 8;

    if (side == Color::WHITE) {
        // Avance simple
        if (rank < 7) {
            if ((empty_squares >> (square + 8)) & 1) {
                if (rank == 6)
                    moves.push_back({square, square + 8, MoveType::PROMOTION, false});
                else
                    moves.push_back({square, square + 8, MoveType::NORMAL, false});

                // Avance double depuis la 2e rangée
                if (rank == 1 && ((empty_squares >> (square + 16)) & 1))
                    moves.push_back({square, square + 16, MoveType::NORMAL, false});
            }
        }

        // Captures diagonales
        if (rank < 7) {
            // capture à droite (vers file + 1)
            if (file < 7) {
                int target = square + 9;
                if ((_pos.colors[opponent] >> target) & 1)
                    moves.push_back({square, target, (rank == 6 ? MoveType::PROMOTION : MoveType::NORMAL), true});
                else if (_pos.en_passant_square != -1 && _pos.en_passant_square == target)
                    moves.push_back({square, target, MoveType::EN_PASSANT, true});
            }

            // capture à gauche (vers file - 1)
            if (file > 0) {
                int target = square + 7;
                if ((_pos.colors[opponent] >> target) & 1)
                    moves.push_back({square, target, (rank == 6 ? MoveType::PROMOTION : MoveType::NORMAL), true});
                else if (_pos.en_passant_square != -1 && _pos.en_passant_square == target)
                    moves.push_back({square, target, MoveType::EN_PASSANT, true});
            }
        }
    }

    else {  // BLACK
        if (rank > 0) {
            if ((empty_squares >> (square - 8)) & 1) {
                if (rank == 1)
                    moves.push_back({square, square - 8, MoveType::PROMOTION, false});
                else
                    moves.push_back({square, square - 8, MoveType::NORMAL, false});

                if (rank == 6 && ((empty_squares >> (square - 16)) & 1))
                    moves.push_back({square, square - 16, MoveType::NORMAL, false});
            }
        }

        // Captures diagonales
        if (rank > 0) {
            // capture à droite (vers file + 1)
            if (file < 7) {
                int target = square - 7;
                if ((_pos.colors[opponent] >> target) & 1)
                    moves.push_back({square, target, (rank == 1 ? MoveType::PROMOTION : MoveType::NORMAL), true});
                else if (_pos.en_passant_square != -1 && _pos.en_passant_square == target)
                    moves.push_back({square, target, MoveType::EN_PASSANT, true});
            }

            // capture à gauche (vers file - 1)
            if (file > 0) {
                int target = square - 9;
                if ((_pos.colors[opponent] >> target) & 1)
                    moves.push_back({square, target, (rank == 1 ? MoveType::PROMOTION : MoveType::NORMAL), true});
                else if (_pos.en_passant_square != -1 && _pos.en_passant_square == target)
                    moves.push_back({square, target, MoveType::EN_PASSANT, true});
            }
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
    int fromX = square % 8;

    int count;
    for (int d : directions) {
        count = 1;
        while (true) {
            int to = square + d * count;

            int toX = to % 8;
            if (to < 0 || to > 63) break;

            if ((d == 1 || d == -1) && std::abs(toX - fromX) != count) break;

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

    int fromX = square % 8;
    int fromY = square / 8;

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (x == 0 && y == 0) continue;
            int toX = fromX + x;
            int toY = fromY + y;

            if (toX < 0 || toX > 7 || toY < 0 || toY > 7) continue;
            int to = square + x + y * 8;

            uint64_t mask = 1ULL << to;
            if (_pos.colors[side] & mask) continue;
            if (_pos.colors[opponent] & mask)
                moves.push_back({square, to, MoveType::NORMAL, true});
            else
                moves.push_back({square, to, MoveType::NORMAL, false});
        }
    }

    uint64_t all_pieces = side | _pos.colors[opponent];
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
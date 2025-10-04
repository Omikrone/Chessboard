// move_generator.cpp

#include "rules/move_generator.hpp"


std::vector<Move> MoveGenerator::all_possible_moves(const Color side, const GameState& game_state, const Bitboards& board) {
    std::vector<Move> moves;
    int64_t mask;
    for (int i = 0; i < 64; i++)
    {
        mask = 1ULL << i;

        if ((game_state.colors[side] & mask) == 0) continue;

        const PieceType piece_type = board.get_piece_type(side, i);
        if (piece_type == PieceType::NONE_PIECE) {continue;}

        std::vector<Move> p_moves = piece_moves(i, side, piece_type, game_state);
        moves.insert(moves.end(), p_moves.begin(), p_moves.end());
    }
    return moves;
}


std::vector<Move> MoveGenerator::piece_moves(const int square, const Color side, const PieceType& piece_type, const GameState& game_state) {
    std::vector<Move> moves;

    Color opponent_side = (side == Color::WHITE) ? Color::BLACK : Color::WHITE;
    switch (piece_type)
    {
    case PieceType::PAWN:
        moves = pawn_moves(square, game_state, side, ~game_state.all_pieces, game_state.colors[opponent_side]);
        break;
    case PieceType::BISHOP:
        moves = MoveGenerator::bishop_moves(square, game_state.colors[side], game_state.colors[opponent_side]);
        break;
    case PieceType::KNIGHT:
        moves = knight_moves(square, game_state.colors[side], game_state.colors[opponent_side]);
        break;
    case PieceType::ROOK:
        moves = rook_moves(square, game_state.colors[side], game_state.colors[opponent_side]);
        break;
    case PieceType::QUEEN:
        moves = queen_moves(square, game_state.colors[side], game_state.colors[opponent_side]);
        break;
    case PieceType::KING:
        moves = king_moves(square, side, game_state.colors[side], game_state.colors[opponent_side], game_state.castling_rights);
        break;
    default:
        break;
    }
    return moves;
}


std::vector<Move> MoveGenerator::pawn_moves(const int square, const GameState& game_state, const Color side, const uint64_t empty_squares, const uint64_t opponent_side) {
    std::vector<Move> moves;

    if (side == Color::WHITE) {
        if (square < 48) { //?
            if ((empty_squares >> (square + 8)) & 1) {
                moves.push_back({square, square + 8, MoveType::NORMAL, false});  // One-step forward walk
                if (square <= 15 && square >= 8 && ((empty_squares >> (square + 16)) & 1)) { 
                    moves.push_back({square, square + 16, MoveType::NORMAL, false});   // Two-step forward walk
                }
            }
            if (square % 8 != 7 && ((opponent_side >> (square + 9)) & 1)) {
                moves.push_back({square, square + 9, MoveType::NORMAL, true});
            }
            else if (square % 8 != 7 && (game_state.en_passant_square == square + 9)) moves.push_back({square, square + 9, MoveType::EN_PASSANT, true});
            if (square % 8 != 0 && ((opponent_side >> (square + 7)) & 1)) {
                moves.push_back({square, square + 7, MoveType::NORMAL, true});
            }
            else if (square % 8 != 0 && (game_state.en_passant_square == square + 7)) moves.push_back({square, square + 7, MoveType::EN_PASSANT, true});
        }
        else if (square < 56)
        {
            if ((empty_squares >> (square + 8)) & 1) {
                moves.push_back({square, square + 8, MoveType::PROMOTION, false});  // One-step forward walk
            }
            if (square % 8 != 7 && ((opponent_side >> (square + 9)) & 1)) {
                moves.push_back({square, square + 9, MoveType::PROMOTION, true}); // Right side-take
            }
            if (square % 8 != 0 && ((opponent_side >> (square + 7)) & 1)) {
                moves.push_back({square, square + 7, MoveType::PROMOTION, true}); // Left side-take
            }
        }
        
    }
    if (side == Color::BLACK) {
        if (square >= 16) {
            if ((empty_squares >> (square - 8)) & 1) {
                moves.push_back({square, square - 8, MoveType::NORMAL, false});
                if (square <= 55 && square >= 48 && ((empty_squares >> (square - 16)) & 1)) {
                    moves.push_back({square, square - 16, MoveType::NORMAL, false});
                }
            }
            if (square % 8 != 7 && ((opponent_side >> (square - 9)) & 1)) {
                moves.push_back({square, square - 9, MoveType::NORMAL, true});
            }
            else if (square % 8 != 7 && (game_state.en_passant_square == square - 9)) moves.push_back({square, square - 9, MoveType::EN_PASSANT, true});
            if (square % 8 != 0 && ((opponent_side >> (square - 7)) & 1)) {
                moves.push_back({square, square - 7, MoveType::NORMAL, true});
            }
            else if (square % 8 != 0 && (game_state.en_passant_square == square - 7)) moves.push_back({square, square - 7, MoveType::EN_PASSANT, true});
        }
        else if (square >= 8)
        {
            if ((empty_squares >> (square - 8)) & 1) {
                moves.push_back({square, square - 8, MoveType::PROMOTION, false});
            }
            if (square % 8 != 7 && ((opponent_side >> (square - 9)) & 1)) {
                moves.push_back({square, square - 9, MoveType::PROMOTION, true}); // Right side-take
            }
            if (square % 8 != 0 && ((opponent_side >> (square - 7)) & 1)) {
                moves.push_back({square, square - 7, MoveType::PROMOTION, true}); // Left side-take
            }
        }
        
    }
    return moves;
}


std::vector<Move> MoveGenerator::knight_moves(const int square, const uint64_t side, const uint64_t opponent_side) {
    std::vector<Move> moves;

    const int dx[] = { 2,  1, -1, -2, -2, -1,  1,  2 };
    const int dy[] = { 1,  2,  2,  1, -1, -2, -2, -1 };

    int fromX = square % 8;
    int fromY = square / 8;

    for (int i = 0; i < 8; i++)
    {
        int toX = fromX + dx[i];
        int toY = fromY + dy[i];

        if (toX < 0 || toX > 7 || toY < 0 || toY > 7) continue;

        int to = toY * 8 + toX;
        uint64_t mask = 1ULL << to;
        if (side & mask) continue;

        bool is_capture = opponent_side & mask;
        moves.push_back({ square, (int)to, MoveType::NORMAL, is_capture });
    }
    return moves;
}


std::vector<Move> MoveGenerator::rook_moves(const int square, const uint64_t side, const uint64_t opponent_side) {
    std::vector<Move> moves;

    const int directions[] = {8, 1, -8, -1};
    int fromX = square % 8;
    
    int count;
    for (int d: directions) {
        count = 1;
        while (true)
        {
            int to = square + d * count;

            int toX = to % 8;
            if (to < 0 || to > 63) break;

            if ((d == 1 || d == -1) && std::abs(toX - fromX) != count) break;

            int64_t mask = 1ULL << to;

            if (side & mask) break;
            else if (opponent_side & mask)
            {
                moves.push_back({square, to, MoveType::NORMAL, true});
                break;
            }
            else {
                moves.push_back({square, to, MoveType::NORMAL, false});
                count++;
            }
        }
    }
    return moves;
}


std::vector<Move> MoveGenerator::bishop_moves(const int square, const uint64_t side, const uint64_t opponent_side) {
    std::vector<Move> moves;

    const int directions[] = {7, -7, -9, 9};
    int fromX = square % 8;
    int fromY = square / 8;
    
    int count;
    for (int d: directions) {
        count = 1;
        while (true)
        {
            int to = square + d * count;

            int toX = to % 8;
            int toY = to / 8;
            if (to < 0 || to > 63) break;

            if (std::abs(toX - fromX) != count || std::abs(toY - fromY) != count) break;

            int64_t mask = 1ULL << to;

            if (side & mask) break;
            else if (opponent_side & mask)
            {
                moves.push_back({square, to, MoveType::NORMAL, true});
                break;
            }
            else {
                moves.push_back({square, to, MoveType::NORMAL, false});
                count++;
            }
        }
    }
    return moves;
}


std::vector<Move> MoveGenerator::queen_moves(const int square, const uint64_t side, const uint64_t opponent_side) {
    std::vector<Move> bishop_moves = MoveGenerator::bishop_moves(square, side, opponent_side);
    std::vector<Move> rook_moves = MoveGenerator::rook_moves(square, side, opponent_side);
    bishop_moves.insert(bishop_moves.end(), rook_moves.begin(), rook_moves.end());
    return bishop_moves;
}


std::vector<Move> MoveGenerator::king_moves(const int square, const Color side_c, const uint64_t side, const uint64_t opponent_side, uint8_t castling_rights) {
    std::vector<Move> moves;
    
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            int to = square + x + y * 8;
            if (to < 0 || to > 63) continue;
            
            uint64_t mask = 1ULL << to;
            if (side & mask) continue;
            if (opponent_side & mask) moves.push_back({square, to, MoveType::NORMAL, true});
            else moves.push_back({square, to, MoveType::NORMAL, false});
        }
    }

    uint64_t all_pieces = side | opponent_side;
    if ((castling_rights >> (side_c * 2 + 1) & 1) 
    && ((all_pieces >> (square + 1)) & 1) == 0 
    && ((all_pieces >> (square + 2)) & 1) == 0) {
        moves.push_back({square, square + 2, MoveType::CASTLE_KINGSIDE, false});
    }
    if ((castling_rights >> (side_c * 2 + 2) & 1) 
    && ((all_pieces >> (square - 1)) & 1) == 0 
    && ((all_pieces >> (square - 2)) & 1) == 0 
    && ((all_pieces >> (square - 3)) & 1) == 0) {
        moves.push_back({square, square - 2, MoveType::CASTLE_QUEENSIDE, false});
    }

    return moves;
}
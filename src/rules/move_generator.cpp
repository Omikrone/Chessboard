// move_generator.cpp

#include "rules/move_generator.hpp"


std::vector<Move> MoveGenerator::pawn_moves(const uint8_t square, const Color side, const uint64_t empty_squares, const uint64_t opponent_side) {
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
                moves.push_back({square, square + 9, MoveType::NORMAL, true}); // Right side-take
            }
            if (square % 8 != 0 && ((opponent_side >> (square + 7)) & 1)) {
                moves.push_back({square, square + 7, MoveType::NORMAL, true}); // Left side-take
            }
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
                moves.push_back({square, square - 9, MoveType::NORMAL, true}); // Right side-take
            }
            if (square % 8 != 0 && ((opponent_side >> (square - 7)) & 1)) {
                moves.push_back({square, square - 7, MoveType::NORMAL, true}); // Left side-take
            }
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


std::vector<Move> MoveGenerator::knight_moves(const uint8_t square, const uint64_t side, const uint64_t opponent_side) {
    std::vector<Move> moves;

    const int8_t dx[] = { 2,  1, -1, -2, -2, -1,  1,  2 };
    const int8_t dy[] = { 1,  2,  2,  1, -1, -2, -2, -1 };

    int8_t fromX = square % 8;
    int8_t fromY = square / 8;

    for (int i = 0; i < 8; i++)
    {
        int8_t toX = fromX + dx[i];
        int8_t toY = fromY + dy[i];

        if (toX < 0 || toX > 7 || toY < 0 || toY > 7) continue;

        int8_t to = toY * 8 + toX;
        uint64_t mask = 1ULL << to;
        if (side & mask) continue;

        bool isCapture = opponent_side & mask;
        moves.push_back({ square, (uint8_t)to, MoveType::NORMAL, isCapture });
    }
    return moves;
}


std::vector<Move> MoveGenerator::rook_moves(const uint8_t square, const uint64_t side, const uint64_t opponent_side) {
    std::vector<Move> moves;

    const int8_t directions[] = {8, 1, -8, -1};
    int fromX = square % 8;
    int fromY = square / 8;
    
    int8_t count;
    for (int8_t d: directions) {
        count = 1;
        while (true)
        {
            int8_t to = square + d * count;

            int toX = to % 8;
            int toY = to / 8;
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


std::vector<Move> MoveGenerator::rook_moves(const uint8_t square, const uint64_t side, const uint64_t opponent_side) {
    std::vector<Move> moves;

    const int8_t directions[] = {7, -7, -9, 9};
    int fromX = square % 8;
    int fromY = square / 8;
    
    int8_t count;
    for (int8_t d: directions) {
        count = 1;
        while (true)
        {
            int8_t to = square + d * count;

            int toX = to % 8;
            int toY = to / 8;
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

std::vector<Move> MoveGenerator::queen_moves(const uint8_t square, const uint64_t side, const uint64_t opponent_side) {
    std::vector<Move> bishop_moves = MoveGenerator::bishop_moves(square, side, opponent_side);
    std::vector<Move> rook_moves = MoveGenerator::rook_moves(square, side, opponent_side);
    bishop_moves.insert(bishop_moves.end(), rook_moves.begin(), rook_moves.end());
    return bishop_moves;
}


std::vector<Move> MoveGenerator::king_moves(const uint8_t square, const uint64_t side, const uint64_t opponent_side) {
    std::vector<Move> moves;

    const int8_t directions[] = {7, -7, -9, 9};
    
    int8_t count;
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            uint8_t to = square + x + y * 8;
            if (to < 0 || to > 63) continue;
            
            uint64_t mask = 1ULL << to;
            if (side & mask) continue;
            if (opponent_side & mask) moves.push_back({square, to, MoveType::NORMAL, true});
            else moves.push_back({square, to, MoveType::NORMAL, false});
        }
    }
    return moves;
}
// move_generator.cpp

#include "rules/move_generator.hpp"


std::vector<Move> MoveGenerator::pawn_moves(const uint8_t square, const Color side, const uint64_t empty_squares, const uint64_t opponent_side) {
    uint64_t moves = 0ULL;

    if (side == Color::WHITE) {
        if ((square + 8) < 64 ) { //?
            if ((empty_squares >> (square + 8)) & 1) {
                moves |= 1ULL << (square + 8);  // One-step forward walk
                if (square <= 15 && square >= 8 && ((empty_squares >> (square + 16)) & 1)) { 
                    moves |= 1ULL << (square + 16);  // Two-step forward walk
                }
            }
            if (square % 8 != 7 && ((opponent_side >> (square + 9)) & 1)) {
                moves |= 1ULL << (square + 9); // Right side-take
            }
            if (square % 8 != 0 && ((opponent_side >> (square + 7)) & 1)) {
                moves |= 1ULL << (square + 7); // Left side-take
            }
        }
    }
    if (side == Color::BLACK) {
        if (square >= 8) {
            if ((empty_squares >> (square - 8)) & 1) {
                moves |= 1ULL << (square - 8);
                if (square <= 55 && square >= 48 && ((empty_squares >> (square - 16)) & 1)) {
                    moves |= 1ULL << (square - 16);
                }
            }
            if (square % 8 != 7 && ((opponent_side >> (square - 9)) & 1)) {
                moves |= 1ULL << (square - 9); // Right side-take
            }
            if (square % 8 != 0 && ((opponent_side >> (square - 7)) & 1)) {
                moves |= 1ULL << (square - 7); // Left side-take
            }
        }
    }
    return moves;
}
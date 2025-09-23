// galme_constants.hpp

#pragma once

#include <cstdint>

#define BOARD_LENGTH 8

enum class EndGame {
    CONTINUING,
    CHECKMATE,
    STALEMATE
};

enum Color {
    WHITE = 0,
    BLACK = 1,
    NONE = 2
};

enum PieceType {
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5,
    NONE_PIECE = 6
};
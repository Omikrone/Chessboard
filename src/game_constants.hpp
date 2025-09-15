// galme_constants.hpp

#pragma once

#define BOARD_LENGTH 8

enum class GameState {
    CONTINUING,
    CHECKMATE,
    STALEMATE
};

enum class Color {
    WHITE = 1,
    BLACK = -1
};

enum class PieceType {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};
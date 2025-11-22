// game_constants.hpp

#pragma once

#define BOARD_LENGTH 8

enum class GameState {
    CONTINUING,
    CHECKMATE,
    STALEMATE,
    DRAW_BY_INSUFFICIENT_MATERIAL,
    DRAW_BY_FIFTY_MOVE_RULE,
    DRAW_BY_75_MOVE_RULE,
    DRAW_BY_THREEFOLD_REPETITION
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
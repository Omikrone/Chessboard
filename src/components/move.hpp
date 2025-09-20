// move.hpp

#pragma once

#include <cstdint>


/**
 * @brief Represents the differents moves type.
 */
enum class MoveType {
    NORMAL,
    EN_PASSANT,
    CASTLE_KINGSIDE,
    CASTLE_QUEENSIDE,
    PROMOTION
};


/**
 * @brief Represents a move on the chess board.
 */
struct Move
{
    uint8_t from;   // Initial position of the piece
    uint8_t to;     // Destination of the piece
    MoveType type;  // Type of the move
    bool take;      // If the move is meant to take a piece

    bool operator==(const Move& other) const {
        return (this->from == other.from && this->to == other.to);
    }
};

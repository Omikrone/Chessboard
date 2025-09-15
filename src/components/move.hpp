// move.hpp

#pragma once

#include "game/components/square.hpp"


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
    Square initPos; // Initial position of the piece
    Square destPos; // Destination of the piece
    MoveType type;  // Type of the move
    bool take;      // If the move is meant to take a piece

    bool operator==(const Move& other) const {
        return (this->initPos == other.initPos && this->destPos == other.destPos);
    }
};

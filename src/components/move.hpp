// move.hpp

#pragma once

#include <cstdint>
#include <iostream>
#include <string>


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
    int from;   // Initial position of the piece
    int to;     // Destination of the piece
    MoveType type;  // Type of the move
    bool take;      // If the move is meant to take a piece

    bool operator==(const Move& other) const {
        return (this->from == other.from && this->to == other.to);
    }

    void print() const {
        std::cout << "{" << std::to_string(this->from) << " : " << std::to_string(this->to) << "}" << std::endl;
    }
};


struct UndoMove
{
    Move move;

    Color side_to_move = Color::WHITE;
    uint8_t castling_rights = UINT8_MAX;
    int en_passant_square = -1;
    int halfmove_clock = 0;
    int fullmove_number = 0;
    PieceType taken_piece= PieceType::NONE_PIECE;
    uint64_t zobrist_hash = UINT64_MAX;
};

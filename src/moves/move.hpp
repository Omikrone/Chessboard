// move.hpp

#pragma once

#include <cstdint>
#include <iostream>
#include <string>

#include "game/game_constants.hpp"

/**
 * @brief Represents the differents moves type.
 */
enum class MoveType { NORMAL, EN_PASSANT, CASTLE_KINGSIDE, CASTLE_QUEENSIDE, PROMOTION };

/**
 * @brief Represents a move on the chess board.
 */
struct Move {
    int from;       // Initial position of the piece
    int to;         // Destination of the piece
    MoveType type;  // Type of the move
    bool take;      // If the move is meant to take a piece
    PieceType promotion_type = PieceType::NONE_PIECE;  // Promotion type if the move is a promotion

    bool operator==(const Move &other) const { return (this->from == other.from && this->to == other.to); }

    bool operator!=(const Move &other) const { return (this->from != other.from || this->to != other.to); }

    static Move from_uci(const std::string &uci) {
        Move move;
        move.from = (uci[1] - '1') * 8 + (uci[0] - 'a');
        move.to = (uci[3] - '1') * 8 + (uci[2] - 'a');
        move.type = MoveType::NORMAL;
        move.take = false;
        if (uci.length() == 5) {
            move.type = MoveType::PROMOTION;
            switch (uci[4]) {
                case 'q':
                    move.promotion_type = PieceType::QUEEN;
                    break;
                case 'r':
                    move.promotion_type = PieceType::ROOK;
                    break;
                case 'b':
                    move.promotion_type = PieceType::BISHOP;
                    break;
                case 'n':
                    move.promotion_type = PieceType::KNIGHT;
                    break;
                default:
                    move.promotion_type = PieceType::NONE_PIECE;
                    break;
            }
        }
        return move;
    }

    std::string to_uci() const {
        std::string uci;
        uci += ('a' + (this->from % 8));
        uci += ('1' + (char)(this->from / 8));
        uci += ('a' + (this->to % 8));
        uci += ('1' + (char)(this->to / 8));
        if (this->type == MoveType::PROMOTION) {
            switch (this->promotion_type) {
                case PieceType::QUEEN:
                    uci += 'q';
                    break;
                case PieceType::ROOK:
                    uci += 'r';
                    break;
                case PieceType::BISHOP:
                    uci += 'b';
                    break;
                case PieceType::KNIGHT:
                    uci += 'n';
                    break;
                default:
                    break;
            }
        }
        return uci;
    }

    void print() const {
        std::cout << "{" << std::to_string(this->from) << " : " << std::to_string(this->to) << "}" << std::endl;
    }
};

/**
 * @brief Represents the information needed to undo a move.
 */
struct UndoMove {
    Move move;

    Color side_to_move = Color::WHITE;
    uint8_t castling_rights = UINT8_MAX;
    int en_passant_square = -1;
    int halfmove_clock = 0;
    int fullmove_number = 0;
    PieceType taken_piece = PieceType::NONE_PIECE;
    uint64_t zobrist_hash = UINT64_MAX;
};

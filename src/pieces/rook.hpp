// rook.hpp

#pragma once

#include "game/pieces/piece.hpp"
#include "game/game_constants.hpp"

#include <vector>
#include <cstdint>

/**
 * @brief Chess rook.
 *
 * Represents the rook piece of chess.
 */
class Rook: public Piece {

    public:

        Rook(PieceType pieceType, const Square &startPosition, Color color);

        std::vector<std::vector<Square>> get_raw_moves() const override;

        static std::vector<std::vector<Square>> get_rook_moves(const Square &currentPosition);

        std::unique_ptr<Piece> clone() const override;
        
        char to_fen() const override;
};
// pawn.hpp

#pragma once

#include "game/pieces/piece.hpp"
#include "game/game_constants.hpp"

#include <vector>
#include <cstdint>

/**
 * @brief Chess pawn.
 *
 * Represents the pawn piece of chess.
 */
class Pawn : public Piece {

    public:

        Pawn(PieceType pieceType, const Square &startPosition, Color color);

        std::vector<std::vector<Square>> get_raw_moves() const override;

        std::unique_ptr<Piece> clone() const;

        char to_fen() const override;
};
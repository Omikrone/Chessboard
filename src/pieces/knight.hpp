// knight.hpp

#pragma once

#include "game/pieces/piece.hpp"
#include "game/game_constants.hpp"

#include <vector>
#include <cstdint>


/**
 * @brief Chess knight.
 *
 * Represents the knight piece of chess.
 */
class Knight: public Piece {

    public:
    
    Knight(PieceType pieceType, const Square &startPosition, Color color);

    std::vector<std::vector<Square>> get_raw_moves() const override;

    std::unique_ptr<Piece> clone() const override;

    char to_fen() const override;
};
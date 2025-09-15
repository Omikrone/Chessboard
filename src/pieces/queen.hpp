// queen.hpp

#pragma once

#include "game/pieces/piece.hpp"
#include "game/pieces/rook.hpp"
#include "game/pieces/bishop.hpp"
#include "game/game_constants.hpp"

#include <vector>
#include <cstdint>


/**
 * @brief Chess queen.
 *
 * Represents the queen piece of chess.
 */
class Queen: public Piece {

    public:

        Queen(PieceType pieceType, const Square &startPosition, Color color);

        std::vector<std::vector<Square>> get_raw_moves() const override;

        std::unique_ptr<Piece> clone() const override;

        char to_fen() const override;
};
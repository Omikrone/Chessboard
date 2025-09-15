// piece.hpp

#pragma once

#include "game/game_constants.hpp"
#include "game/components/square.hpp"
#include "game/components/move.hpp"

#include <memory>
#include <vector>


/**
 * @brief Abstract class representing a piece of a chess game.
 *
 * Define the common interface for all the pieces:
 * - common properties like the color or the piece type
 * - generate the movements specific to a piece
 * - Get the FEN symbol of the piece
 */
class Piece {
    public:

        PieceType _pieceType;
        Square _position;
        Color _color;
        bool _hasMoved = false;

        Piece(PieceType pieceType, const Square& position, Color color)
            : _pieceType(pieceType), _position(position), _color(color) {}

        virtual ~Piece() = default;

        /**
         * @brief Generates the specific moves for the piece.
         * 
         * @return A vector of vectors of squares representing the differents directions for the piece.
         */
        virtual std::vector<std::vector<Square>> get_raw_moves() const = 0;

        /**
         * @brief Clones the piece into a deep copy for simulations.
         * 
         * @return A unique pointer to the new clone piece.
         */
        virtual std::unique_ptr<Piece> clone() const = 0;

        /**
         * @brief Clones the piece into a deep copy for simulations.
         * 
         * @return A char representing the fen notation of the piece.
         */
        virtual char to_fen() const = 0;
};

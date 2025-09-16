// board.hpp

#pragma once

#include "game/pieces/piece.hpp"
#include "game/pieces/pawn.hpp"
#include "game/pieces/rook.hpp"
#include "game/pieces/bishop.hpp"
#include "game/pieces/knight.hpp"
#include "game/pieces/king.hpp"
#include "game/pieces/queen.hpp"
#include "game/components/move.hpp"
#include "game/components/square.hpp"
#include "game/game_constants.hpp"

#include <memory>
#include <iostream>


/**
 * @brief Class representing a chess board.
 *
 * Manages the pieces, the moves adn the checks ont the king.
 */
class GameBoard {

    private:

        uint64_t _white_pawns, _white_knights, _white_bishops, _white_rooks, _white_queens, _white_king;
        uint64_t _black_pawns, _black_knights, _black_bishops, _black_rooks, _black_queens, _black_king;
        uint64_t _all_white_pieces, _all_black_pieces;
        uint64_t _all_pieces;

        /**
         * @brief Moves a piece on the board.
         *
         * @param from Initial position of the piece.
         * @param to Position where to move the piece.
         */
        void move_piece(Square from, Square to);

        /**
         * @brief Applies the En Passant move to a piece.
         *
         * @param pawn The piece to move (a pawn).
         * @param move The specific move to apply.
         */
        void enpassant(Piece *pawn, Move move);

        /**
         * @brief Applies the kingside castle move to a piece.
         *
         * @param king The piece to move (a king).
         */
        void kingside_castle(Color side);

        /**
         * @brief Applies the queenside castle move to a piece.
         *
         * @param king The piece to move (a king).
         */
        void queenside_castle(Color side);

        /**
         * @brief Promotes a piece.
         *
         * @param pawnToPromote The piece to promote (a pawn).
         */
        void promotion(Piece *pawnToPromote, PieceType pieceType);

    public:

        GameBoard();
        GameBoard(const GameBoard&) = delete;

        Color get_color_at(Square sq) const;

        /**
         * @brief Gets a piece on the game board.
         *
         * @param sq Position of the piece.
         * @return A pointer to the piece.
         */
        Piece *get_piece_at(Square sq) const;

        /**
         * @brief Applies a given move on the game board.
         *
         * @param move The move to apply.
         */
        void make_move(const Move& move);

        /**
         * @brief Gets a king on the game board.
         *
         * @param kingColor The color of the king.
         */
        King& get_king(Color kingColor);

        /**
         * @brief Checks if a square is attacked by a piece.
         *
         * @param enemyMoves All the possible moves for the other player.
         * @param position The square to check.
         */
        bool is_square_attacked(std::vector<Move>& enemyMoves, Square position);

        /**
         * @brief Creates a deep copy of the game board.
         *
         * @return An unique pointer to new game board.
         */
        std::unique_ptr<GameBoard> clone() const;

        /**
         * @brief Displays the game board in ASCII mode (for debugging).
         *
         */
        void print_board();
};

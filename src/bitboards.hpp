// bitboards.hpp

#pragma once

#include "game_constants.hpp"

#include <cstdint>


// TODO: define an intern type Bitboard
struct GameState
{
    uint64_t pieces[2][6];
    uint64_t colors[2];
    uint64_t all_pieces;

    Color side_to_move = Color::WHITE;
    uint8_t castling_rights = UINT8_MAX;
    int en_passant_square = -1;
    int halfmove_clock = 0;
    int fullmove_number = 0;
};


/**
 * @brief Main class representing a chess board with bitboards notations.
 *
 * Manages the board, the application of moves, and the consistency between all the bitboards.
 */
class Bitboards
{
    private:

        GameState& _game_state;

        /**
         * @brief Updates a side bitboard based on its piece bitboards.
         *
         * @param side Side bitboard to update.
         */
        void update_side(const Color side);
        
        /**
         * @brief Updates the global bitboard based on the side bitboards.
         *
         */
        void update_all();

    public:

        Bitboards(GameState& game_state);

        ~Bitboards() = default;
        
        /**
         * @brief Adds a piece to the bitboards.
         *
         * @param side Color of the new piece.
         * @param piece_type Type of the new piece.
         * @param at Initial position of the new piece.
         */
        void add_piece(const Color side, const PieceType piece_type, const int at);

        /**
         * @brief Removes a piece from the bitboards.
         *
         * @param side Color of the piece.
         * @param piece_type Type of the piece.
         * @param at Current position of the piece.
         */
        void remove_piece(const Color side, const PieceType piece_type, const int at);

        /**
         * @brief Moves a piece on the bitboards.
         *
         * @param side Color of the piece.
         * @param piece_type Type of the piece.
         * @param from Current position of the piece.
         * @param to Destination of the piece.
         */
        void move_piece(const Color side, const PieceType piece_type, const int from, const int to);

        /**
         * @brief Checks if a square is occupied by a piece.
         *
         * @param at Position to check.
         * @return The color of the piece that occupies the square.
         */
        const Color is_occupied(const int at) const;

        /**
         * @brief Gets the type of the piece at a given position.
         *
         * @param side Color of side to check.
         * @param at The position to check.
         * @return The type of the piece that occupies the square.
         */
        const PieceType get_piece_type(const Color side, const int at) const;

        /**
         * @brief Prints a bitboard for debugging.
         *
         * @param bb The bitboard to print.
         */
        void print_board(uint64_t bb) const;
};
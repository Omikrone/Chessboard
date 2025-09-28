// move_executor.hpp

#pragma once

#include "bitboards.hpp"
#include "components/move.hpp"
#include "game_history.hpp"


/**
 * @brief Class meant for executing moves on bitboards.
 *
 * Handles and executes moves and special moves like EnPassant, castling and promotion.
 */
class MoveExecutor
{
    private:

        GameHistory& _history;
        GameState& _game_state;
        Bitboards& _board;

        /**
         * @brief Executes a normal move on the board.
         *
         * @param side Player which wants to apply the move.
         * @param move Move to apply.
         */
        void normal(const Color side, const Move& move);

        /**
         * @brief Applies a kingside castle on the board.
         *
         * @param side Player which wants to apply the move.
         * @param move Move to apply.
         */
        void castle_kingside(const Color side, const Move& move);

        /**
         * @brief Applies a queenside castle on the board.
         *
         * @param side Player which wants to apply the move.
         * @param move Move to apply.
         */
        void castle_queenside(const Color side, const Move& move);

        /**
         * @brief Applies an en passant move on the board.
         *
         * @param side Player which wants to apply the move.
         * @param move Move to apply.
         */
        void en_passant(const Color side, const Move& move);

        /**
         * @brief Applies a pawn promotion on the board.
         *
         * @param side Player which wants to apply the move.
         * @param square Position of the pawn to promote.
         * @param new_piece New piece to replace the pawn with.
         */
        void promotion(const Color side, const int square, const PieceType new_piece);

    public:

        MoveExecutor(GameHistory& history, GameState& state, Bitboards& board);

        ~MoveExecutor() = default;

        /**
         * @brief Applies a move on the game board.
         *
         * @param side Player which wants to apply the move.
         * @param move Move to apply.
         */
        void make_move(const Color side, const Move& move);

        /**
         * @brief Restores the previous game state before the last move.
         * 
         */
        void unmake_last_move();
};
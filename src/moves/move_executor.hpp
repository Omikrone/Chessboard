// move_executor.hpp

#pragma once

#include "board/bitboards.hpp"
#include "board/zobrist.hpp"
#include "game/game_history.hpp"
#include "moves/move.hpp"

/**
 * @brief Class meant for executing moves on bitboards.
 *
 * Handles and executes moves and special moves like EnPassant, castling and
 * promotion.
 */
class MoveExecutor {
   private:
    GameHistory &_history;
    Position &_position;
    Bitboards &_board;
    Zobrist &_zobrist;

    /**
     * @brief Executes a normal move on the board.
     *
     * @param side Player which wants to apply the move.
     * @param move Move to apply.
     */
    void normal(UndoMove &undo, const Color side, const Move &move);

    void undo_normal(const Color side, const Move &move, PieceType taken_piece);

    /**
     * @brief Applies a kingside castle on the board.
     *
     * @param side Player which wants to apply the move.
     * @param move Move to apply.
     */
    void castle_kingside(const Color side, const Move &move);

    void undo_castle_kingside(const Color side, const Move &move);

    /**
     * @brief Applies a queenside castle on the board.
     *
     * @param side Player which wants to apply the move.
     * @param move Move to apply.
     */
    void castle_queenside(const Color side, const Move &move);

    /**
     * @brief Undo the queenside castle on the board.
     *
     * @param side Player which applied the move.
     * @param move Move to cancel.
     */
    void undo_castle_queenside(const Color side, const Move &move);

    /**
     * @brief Applies an en passant move on the board.
     *
     * @param undo The undo structure to fill.
     * @param side Player which wants to apply the move.
     * @param move Move to apply.
     */
    void en_passant(const Color side, const Move &move);

    /**
     * @brief Cancels an en passant move on the board.
     *
     * @param side Player which applied the move.
     * @param move The move to cancel.
     */
    void undo_en_passant(const Color side, const Move &move, int en_passant_square);

    /**
     * @brief Applies a pawn promotion on the board.
     *
     * @param side Player which wants to apply the move.
     * @param square Position of the pawn to promote.
     * @param new_piece New piece to replace the pawn with.
     */
    void promotion(const Color side, const int square, const PieceType new_piece);

    /**
     * @brief Cancels a pawn promotion on the board.
     *
     * @param side Player which wants to apply the move.
     * @param move The Move to cancel.
     */
    void undo_promotion(const Color side, const Move &move);

   public:
    MoveExecutor(GameHistory &history, Position &state, Bitboards &board, Zobrist &zobrist);

    ~MoveExecutor() = default;

    /**
     * @brief Applies a move on the game board.
     *
     * @param side Player which wants to apply the move.
     * @param move The move to apply.
     */
    void make_move(const Color side, const Move &move, const PieceType promotion_piece = PieceType::NONE_PIECE);

    /**
     * @brief Cancels the last move made and restores the previous game state
     * before.
     *
     */
    void unmake_last_move();
};
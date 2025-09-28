// move_validator.hpp

#pragma once

#include "bitboards.hpp"
#include "components/move.hpp"
#include "rules/move_generator.hpp"
#include "rules/move_executor.hpp"

#include <bit>
#include <vector>


/**
 * @brief Class meant for the validation of the raw moves generated.
 *
 * Manages the board, the history of moves, the current turn and the state of the game.
 */
class MoveValidator
{
    private:

        GameState& _game_state;
        Bitboards& _board;
        MoveExecutor& _executor;

        /**
         * @brief Checks if a square is attacked by the opponent.
         *
         * @param square The position to check.
         * @param opponent The color of the opponent.
         * @return true if the square is attacked, else false.
         */
        bool is_square_attacked(const int square, const Color opponent) const;

        /**
         * @brief Checks if a king is in check by the opponent.
         *
         * @param king_color The color of the king.
         * @return true if the king is in check, else false.
         */
        bool is_king_in_check(const Color king_color) const;

        /**
         * @brief Checks if a castle move is legal or not.
         *
         * @param move The castle move.
         * @param king_color The color of the king that wants to castle.
         * @return true if the castle move is legal, else false.
         */
        bool check_castle(const Move& move, const Color king_color) const;

        /**
         * @brief Gets the position of a king on the chess board.
         *
         * @param king_color The color of the king to find.
         * @return The position of the king.
         */
        int find_king(const Color king_color) const;

    public:

        MoveValidator(GameState& position, Bitboards& board, MoveExecutor& executor);

        ~MoveValidator() = default;

        /**
         * @brief Checks if a move is legal or not.
         *
         * @param move The move to check.
         * @return true if the move is legal, else false.
         */
        bool is_legal(const Move& move);
};
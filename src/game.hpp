// game.hpp

#pragma once

#include "rules/move_generator.hpp"
#include "rules/move_validator.hpp"
#include "rules/move_executor.hpp"
#include "bitboards.hpp"
#include "game_constants.hpp"
#include "fen.hpp"

#include <string>


/**
 * @brief Main class representing a chess game.
 *
 * Manages the board, the history of moves, the current turn and the state of the game.
 */
class Game
{
    private:
      
        Bitboards _board;
        GameState _game_state;
        MoveValidator _validator;
        MoveExecutor _executor;
        GameHistory _history;

    public:

        Game();

        ~Game() = default;
        
        /**
         * @brief Tries to apply a given move on the game board.
         *
         * @param move Move to apply.
         * @return true if the move is legal, else false.
         */
        bool try_apply_move(const int from, const int to);

        /**
         * @brief Switch the current turn.
         * 
         */
        void next_turn();

        /**
         * @brief Gets the state of the game.
         * 
         * @return The state of the game (checkmate, ...).
         */
        EndGame get_game_state();

        /**
         * @brief Gets the the color of the current player.
         * 
         * @return The Color of the current turn.
         */
        Color get_current_turn() const;

        /**
         * @brief Gets the current number of moves played by a side during the game.
         * 
         * @param side The color ot get the number of moves.
         * @return The number of moves.
         */
        int get_nb_moves(const Color side) const;

        /**
         * @brief Converts the current game state into its FEN notation.
         * 
         * @return A string represnting the current game state.
         */
        std::string get_fen() const;
};
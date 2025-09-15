// game.hpp

#pragma once

#include "game/board/board.hpp"
#include "game/rules/move_validator.hpp"
#include "game/rules/move_generator.hpp"
#include "game/components/square.hpp"
#include "game/game_history.hpp"
#include "game/game_constants.hpp"


/**
 * @brief Main class representing a chess game.
 *
 * Manages the board, the history of moves, the current turn and the state of the game.
 */
class Game
{
    private: 
      
        GameBoard _board;
        MoveValidator _moveValidator;
        GameHistory _history;
        Color _currentTurn;
        int _blackMovesNb;
        int _whiteMovesNb;

    public:

        Game();

        ~Game() = default;
        
        /**
         * @brief Tries to apply a given move on the game board.
         *
         * @param move Move to apply.
         * @return true if the move is legal, else false.
         */
        bool try_apply_move(const Move& move);

        /**
         * @brief Gets the legal moves from a square.
         *
         * @param sq Square of the initial position.
         * @return A vector of the possible (legal) moves to play.
         */
        std::vector<Move> get_legal_moves(const Square sq);

        /**
         * @brief Switch the current turn.
         */
        void next_turn();

        /**
         * @brief Gets the state of the game.
         * 
         * @return The game of the state (checkmate, ...).
         */
        GameState get_game_state();

        /**
         * @brief Gets the the color of the current player.
         * 
         * @return The Color of the current turn.
         */
        Color get_current_turn() const;

        /**
         * @brief Gets the game board related to the game.
         * 
         * @return The GameBoard.
         */
        GameBoard& get_game_board();

        /**
         * @brief Gets the current number of moves played in the game.
         * 
         * @return The number of moves.
         */
        int get_nb_moves(const Color side) const;
};
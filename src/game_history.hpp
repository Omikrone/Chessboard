// game_history.hpp

#pragma once

#include "bitboards.hpp"

#include <vector>
#include <cassert>


/**
 * @brief Class representing the history of moves of a chess game.
 *
 */
class GameHistory {

    private:

        std::vector<GameState> _history;

    public:

        GameHistory() = default;

        ~GameHistory() = default;

        /**
         * @brief Adds a move played to the history.
         *
         * @param move Move played.
         */
        void push(const GameState game_state);

        void pop();

        /**
         * @brief Gets the last move played.
         *
         * @return The move.
         */
        const GameState& last() const;

        /**
         * @brief Verifies if the history is empty.
         *
         * @return true if it is empty, else false.
         */
        bool empty() const;
};
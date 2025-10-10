// game_history.hpp

#pragma once

#include "bitboards.hpp"
#include "components/move.hpp"

#include <vector>
#include <cassert>
#include <cstddef>


/**
 * @brief Class representing the history of moves of a chess game.
 *
 */
class GameHistory {

    private:

        std::vector<UndoMove> _history;

    public:

        GameHistory();

        ~GameHistory() = default;

        /**
         * @brief Adds a new game state to the history.
         *
         * @param game_state The current state of the game.
         */
        void push(UndoMove game_state);

        /**
         * @brief Removes the last game state played from the history.
         * 
         */
        UndoMove pop();

        /**
         * @brief Gets the last game state added.
         *
         * @return The state of the game.
         */
        UndoMove last() const;

        /**
         * @brief Verifies if the history is empty.
         *
         * @return true if it is empty, else false.
         */
        bool empty() const;

        /**
         * @brief Gets the size (number of moves) of the history.
         *
         * @return The size of the history.
         */
        size_t size() const;
};
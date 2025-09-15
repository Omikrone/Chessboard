// game_history.hpp

#pragma once

#include "game/components/move.hpp"
#include "game/game_constants.hpp"

#include <vector>
#include <cassert>


/**
 * @brief Class representing the history of moves of a chess game.
 *
 */
class GameHistory {

    private:

        std::vector<Move> _history;

    public:

        GameHistory() = default;

        ~GameHistory() = default;

        /**
         * @brief Adds a move played to the history.
         *
         * @param move Move played.
         */
        void push(Move move);

        /**
         * @brief Gets the last move played.
         *
         * @return The move.
         */
        const Move& last() const;

        /**
         * @brief Verifies if the history is empty.
         *
         * @return true if it is empty, else false.
         */
        bool empty() const;
};
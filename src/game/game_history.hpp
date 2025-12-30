#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

#include "board/bitboards.hpp"
#include "moves/move.hpp"

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
     * @param undo The undo information for the move.
     */
    void push(UndoMove undo);

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

    /**
     * @brief Gets the move at a given index in the history.
     *
     * @param index The index to get the move from.
     * @return The move at the given index.
     */
    UndoMove at(size_t index) const;

    /**
     * @brief Gets all the moves played in the game.
     *
     * @return A vector containing all the moves played.
     */
    std::vector<Move> get_moves() const;
};
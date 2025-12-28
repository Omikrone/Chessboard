// game.hpp

#pragma once

#include <optional>
#include <string>

#include "board/bitboards.hpp"
#include "board/zobrist.hpp"
#include "game/game_constants.hpp"
#include "moves/move_executor.hpp"
#include "moves/move_generator.hpp"
#include "rules/move_validator.hpp"
#include "uci/fen.hpp"

/**
 * @brief Main class representing a chess game.
 *
 * Manages the board, the history of moves, the current turn and the state of
 * the game.
 */
class Game {
   private:
    Position _position;
    Bitboards _board;
    GameHistory _history;
    Zobrist _zobrist;
    MoveGenerator _generator;
    MoveExecutor _executor;
    MoveValidator _validator;

   public:
    Game();

    ~Game() = default;

    /**
     * @brief Tries to apply a given move on the game board.
     *
     * @param move Move to apply.
     * @return true if the move is legal, else false.
     */
    bool try_apply_move(const int from, const int to, const std::optional<PieceType> promotion_piece = std::nullopt);

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
    GameState get_game_state();

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
    int get_fullmove_number() const;

    /**
     * @brief Gets the current halfmove clock of the game.
     *
     * @return The halfmove clock.
     */
    int get_halfmove_clock() const;

    /**
     * @brief Converts the current game state into its FEN notation.
     *
     * @return A string representing the current game state.
     */
    std::string get_fen() const;

    /**
     * @brief Loads a position given by a FEN notation.
     *
     * @param fen The FEN notation of the game to load.
     */
    void load_fen(std::string &fen);

    /**
     * @brief Generates all legal moves for a side of the chessboard.
     *
     * @return A vector of the moves.
     */
    std::vector<Move> get_legal_moves();

    std::vector<Move> get_capture_and_check_moves();

    /**
     * @brief Gets the board of the game.
     *
     * @return The board.
     */
    Bitboards &get_board();

    /**
     * @brief Gets the list of moves played in the game.
     *
     * @return A vector of the moves played.
     */
    std::vector<Move> get_played_moves() const;

    /**
     * @brief Cancels the last move made on the board and restores the previous
     * state.
     *
     */
    void unmake_move();
};
#pragma once

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "board/bitboards.hpp"
#include "game/game_constants.hpp"

/**
 * @brief Static class to respresent a game in the FEN notation.
 *
 * Manages the conversion of the state of a game to the FEN notation and vice
 * versa.
 */
class FEN {
   public:
    /**
     * @brief Converts the state of a game into a FEN notation.
     *
     * @param game Game to convert.
     * @param board Board with the piece informations.
     * @return A string representing the FEN notation.
     */
    static std::string to_string(const Position &game, const Bitboards &board);

    /**
     * @brief Loads a FEN and creates the corresponding game state.
     *
     * @param fen String representing the FEN notation of the game.
     * @param game Game State to fill with the FEN informations.
     * @param board Game Board to fill with the FEN informations.
     *
     * @attention The fen string is considered to be valid, otherwise there may be
     * some bugs.
     */
    static void load(std::string fen, Position &game, Bitboards &board);

    /**
     * @brief Gets the FEN notation of a piece.
     *
     * @param piece_color Color of the piece.
     * @param piece_type Type of the piece.
     * @return A char representing the piece into a FEN notation.
     */
    static char piece_to_symbol(const Color piece_color, const PieceType piece_type);

    /**
     * @brief Converts a symbol in FEN notation to a piece.
     *
     * @param piece_color Color of the piece.
     * @param piece_type Type of the piece.
     * @return A char representing the piece into a FEN notation.
     */
    static PieceType symbol_to_piece(const char piece_symbol);

    /**
     * @brief Gets the FEN notation of the castling rights.
     *
     * @param rights Binary notation of the rights (1 for right of castle, else
     * 0).
     * @return A string representing the castling rights of both sides in the
     * game.
     */
    static std::string castling_rights_to_fen(uint8_t rights);

    /**
     * @brief Gets the FEN notation of the castling rights.
     *
     * @param rights Binary notation of the rights (1 for right of castle, else
     * 0).
     * @return A string representing the castling rights of both sides in the
     * game.
     */
    static uint8_t fen_to_castling_rights(const std::string &fen);
};
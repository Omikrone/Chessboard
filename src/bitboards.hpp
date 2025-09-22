// bitboards.hpp

#pragma once

#include "game_constants.hpp"

#include <cstdint>


// TODO: define an intern type Bitboard
struct GameState
{
    uint64_t pieces[2][6];
    uint64_t colors[2];
    uint64_t all_pieces;

    Color side_to_move;
    uint8_t castling_rights;
    int en_passant_square;
    int halfmove_clock;
    int fullmove_number;
};


class Bitboards
{
private:
    GameState _game_state;

    void update_side(const Color side);
    void update_all();

public:
    Bitboards();
    ~Bitboards() = default;
    
    void add_piece(const Color side, const PieceType piece_type, const uint8_t at);
    void remove_piece(const Color side, const PieceType piece_type, const uint8_t at);
    void move_piece(const Color side, const PieceType piece_type, const uint8_t from, const uint8_t to);
    const Color is_occupied(const uint8_t at) const;
    const PieceType get_piece_type(const Color side, const uint8_t at) const;
};
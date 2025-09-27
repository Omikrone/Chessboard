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

    Color side_to_move = Color::WHITE;
    int castling_rights = INT32_MAX;
    int en_passant_square = -1;
    int halfmove_clock = 0;
    int fullmove_number = 0;
};


class Bitboards
{
private:
    GameState _game_state;

    void update_side(const Color side);
    void update_all();

public:
    Bitboards(GameState& game_state);
    ~Bitboards() = default;
    
    void add_piece(const Color side, const PieceType piece_type, const int at);
    void remove_piece(const Color side, const PieceType piece_type, const int at);
    void move_piece(const Color side, const PieceType piece_type, const int from, const int to);
    const Color is_occupied(const int at) const;
    const PieceType get_piece_type(const Color side, const int at) const;
    void print_board(uint64_t bb);
};
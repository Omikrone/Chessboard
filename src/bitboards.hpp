// bitboards.hpp

#pragma once

#include "game_constants.hpp"

#include <cstdint>


// TODO: define an intern type Bitboard
struct Position
{
    uint64_t pieces[2][6];
    uint64_t colors[2];
    uint64_t all_pieces;
};


class Bitboards
{
private:
    Position _position;

    void update_side(const Color side);
    void update_all();

public:
    Bitboards();
    ~Bitboards() = default;
    
    void add_piece(const Color side, const PieceType piece_type, const uint8_t at);
    void remove_piece(const Color side, const PieceType piece_type, const uint8_t at);
    void move_piece(const Color side, const PieceType piece_type, const uint8_t from, const uint8_t to);
    const Color is_occupied(const uint8_t at) const;
};
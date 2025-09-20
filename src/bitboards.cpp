// bitboards.cpp

#include "bitboards.hpp"


Bitboards::Bitboards() {
        
    // Static initialization of white pieces
    _position.pieces[Color::WHITE][PieceType::PAWN] = 0x000000000000FF00ULL;
    _position.pieces[Color::WHITE][PieceType::KNIGHT] = 0x0000000000000042ULL;
    _position.pieces[Color::WHITE][PieceType::BISHOP] = 0x0000000000000024ULL;
    _position.pieces[Color::WHITE][PieceType::ROOK] = 0x0000000000000081ULL;
    _position.pieces[Color::WHITE][PieceType::QUEEN] = 0x0000000000000008ULL;
    _position.pieces[Color::WHITE][PieceType::KING] = 0x0000000000000010ULL;

    // Static initialization of black pieces
    _position.pieces[Color::BLACK][PieceType::PAWN] = 0x00FF000000000000ULL;
    _position.pieces[Color::BLACK][PieceType::KNIGHT] = 0x4200000000000000ULL;
    _position.pieces[Color::BLACK][PieceType::BISHOP] = 0x2400000000000000ULL;
    _position.pieces[Color::BLACK][PieceType::ROOK] = 0x8100000000000000ULL;
    _position.pieces[Color::BLACK][PieceType::QUEEN] = 0x0800000000000000ULL;
    _position.pieces[Color::BLACK][PieceType::KING] = 0x1000000000000000ULL;

    // Initialization of other positionnal bitboards
    update_side(Color::WHITE);
    update_side(Color::BLACK);
    update_all();
}


void Bitboards::update_side(const Color side) {
    _position.colors[side] = 0ULL;
    for (uint64_t& bb: _position.pieces[side]) {
        _position.colors[side] |= bb;
    }
}


void Bitboards::update_all() {
    _position.all_pieces = 0ULL;
    _position.all_pieces |= _position.colors[Color::WHITE];
    _position.all_pieces |= _position.colors[Color::BLACK];
}


void Bitboards::add_piece(const Color side, const PieceType piece_type, const uint8_t at) {
    _position.pieces[side][piece_type] |= (1ULL << at);
    update_side(side);
    update_all();
}


void Bitboards::remove_piece(const Color side, const PieceType piece_type, const uint8_t at) {
    _position.pieces[side][piece_type] &= ~(1ULL << at);
    update_side(side);
    update_all();
}


void Bitboards::move_piece(const Color side, const PieceType piece_type, const uint8_t from, const uint8_t to) {
    _position.pieces[side][piece_type] &= ~(1ULL << from);
    _position.pieces[side][piece_type] |= (1ULL << to);
    update_side(side);
    update_all();
}


const Color Bitboards::is_occupied(uint8_t at) const {
    uint64_t mask = (1ULL << at);
    if (!(_position.all_pieces & mask)) return Color::NONE;
    return (_position.colors[Color::WHITE] & mask) ? Color::WHITE : Color::BLACK;
}
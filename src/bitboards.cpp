// bitboards.cpp

#include "bitboards.hpp"
#include <iostream>
#include <bitset>


Bitboards::Bitboards(GameState& game_state) : _game_state(game_state) {
        
    // Static initialization of white pieces
    _game_state.pieces[Color::WHITE][PieceType::PAWN] = 0x000000000000FF00ULL;
    _game_state.pieces[Color::WHITE][PieceType::KNIGHT] = 0x0000000000000042ULL;
    _game_state.pieces[Color::WHITE][PieceType::BISHOP] = 0x0000000000000024ULL;
    _game_state.pieces[Color::WHITE][PieceType::ROOK] = 0x0000000000000081ULL;
    _game_state.pieces[Color::WHITE][PieceType::QUEEN] = 0x0000000000000008ULL;
    _game_state.pieces[Color::WHITE][PieceType::KING] = 0x0000000000000010ULL;

    // Static initialization of black pieces
    _game_state.pieces[Color::BLACK][PieceType::PAWN] = 0x00FF000000000000ULL;
    _game_state.pieces[Color::BLACK][PieceType::KNIGHT] = 0x4200000000000000ULL;
    _game_state.pieces[Color::BLACK][PieceType::BISHOP] = 0x2400000000000000ULL;
    _game_state.pieces[Color::BLACK][PieceType::ROOK] = 0x8100000000000000ULL;
    _game_state.pieces[Color::BLACK][PieceType::QUEEN] = 0x0800000000000000ULL;
    _game_state.pieces[Color::BLACK][PieceType::KING] = 0x1000000000000000ULL;

    // Initialization of other positionnal bitboards
    update_side(Color::WHITE);
    update_side(Color::BLACK);
    update_all();
}


void Bitboards::update_side(const Color side) {
    _game_state.colors[side] = 0ULL;
    for (uint64_t bb: _game_state.pieces[side]) {
        _game_state.colors[side] |= bb;
    }
    //std::cout << "SIDEBOARD" << std::endl;
    //print_board(_game_state.colors[side]);
}


void Bitboards::update_all() {
    _game_state.all_pieces = 0ULL;
    _game_state.all_pieces |= _game_state.colors[Color::WHITE];
    _game_state.all_pieces |= _game_state.colors[Color::BLACK];
}


void Bitboards::add_piece(const Color side, const PieceType piece_type, const int at) {
    _game_state.pieces[side][piece_type] |= (1ULL << at);
    update_side(side);
    update_all();
}


void Bitboards::remove_piece(const Color side, const PieceType piece_type, const int at) {
    _game_state.pieces[side][piece_type] &= ~(1ULL << at);
    update_side(side);
    update_all();
}


void Bitboards::move_piece(const Color side, const PieceType piece_type, const int from, const int to) {
    print_board(_game_state.pieces[side][PieceType::PAWN]);
    _game_state.pieces[side][piece_type] &= ~(1ULL << from);
    _game_state.pieces[side][piece_type] |= (1ULL << to);
    update_side(side);
    update_all();
    std::cout << "GAMEBOARD: " << std::endl;
    print_board(_game_state.pieces[side][PieceType::PAWN]);
}


const Color Bitboards::is_occupied(int at) const {
    uint64_t mask = (1ULL << at);
    if (!(_game_state.all_pieces & mask)) return Color::NONE;
    return (_game_state.colors[Color::WHITE] & mask) ? Color::WHITE : Color::BLACK;
}


const PieceType Bitboards::get_piece_type(const Color side, const int at) const {
    uint64_t mask = (1ULL << at);
    for (int i = 0; i < 6; i++)
    {
        if (_game_state.pieces[side][i] & mask) return static_cast<PieceType>(i);
    }
    return PieceType::NONE_PIECE;
}

void Bitboards::print_board(uint64_t bb) const {
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            std::cout << ((bb >> square) & 1ULL ? "1 " : ". ");
        }
        std::cout << std::endl;
    }
}
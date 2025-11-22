// bitboards.cpp

#include "bitboards.hpp"


Bitboards::Bitboards(Position& game_state) : _position(game_state) {
        
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
    for (uint64_t bb: _position.pieces[side]) {
        _position.colors[side] |= bb;
    }
}


void Bitboards::update_all() {
    _position.all_pieces = 0ULL;
    _position.all_pieces |= _position.colors[Color::WHITE];
    _position.all_pieces |= _position.colors[Color::BLACK];
}


void Bitboards::add_piece(const Color side, const PieceType piece_type, const int at) {
    _position.pieces[side][piece_type] |= (1ULL << at);
    update_side(side);
    update_all();
}


void Bitboards::remove_piece(const Color side, const PieceType piece_type, const int at) {
    _position.pieces[side][piece_type] &= ~(1ULL << at);
    update_side(side);
    update_all();
}


void Bitboards::clear() {
    for (int side = 0; side < 2; side++) {
        for (int piece = 0; piece < 6; piece++) {
            _position.pieces[side][piece] = 0ULL;
        }
        _position.colors[side] = 0ULL;
    }
    _position.all_pieces = 0ULL;
}


void Bitboards::move_piece(const Color side, const PieceType piece_type, const int from, const int to) {
    _position.pieces[side][piece_type] &= ~(1ULL << from);
    _position.pieces[side][piece_type] |= (1ULL << to);
    update_side(side);
    update_all();
}


const Color Bitboards::is_occupied(int at) const {
    uint64_t mask = (1ULL << at);
    if (!(_position.all_pieces & mask)) return Color::NONE;
    return (_position.colors[Color::WHITE] & mask) ? Color::WHITE : Color::BLACK;
}


const PieceType Bitboards::get_piece_type(const Color side, const int at) const {
    uint64_t mask = (1ULL << at);
    for (int i = 0; i < 6; i++)
    {
        if (_position.pieces[side][i] & mask) return static_cast<PieceType>(i);
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
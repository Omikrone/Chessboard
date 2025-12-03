// zobrist.cpp

#include "zobrist.hpp"

Zobrist::Zobrist(Position &position, Bitboards &board)
    : _position(position), _board(board) {
  std::mt19937_64 rng(std::time(nullptr));
  std::uniform_int_distribution<uint64_t> dist;
  for (int i = 0; i < 64; i++) {
    for (int j = 0; j < 12; j++) {
      _table[i][j] = dist(rng);
    }
  }
  _turn = dist(rng);
}

uint64_t Zobrist::hash() {
  uint64_t hash = 0;

  if (_position.side_to_move == Color::BLACK)
    hash ^= _turn;
  for (int i = 0; i < 64; i++) {
    Color side = _board.is_occupied(i);
    if (side != Color::NONE) {
      PieceType piece_type = _board.get_piece_type(side, i);
      hash ^= _table[i][piece_type + 6 * side];
    }
  }
  return hash;
}

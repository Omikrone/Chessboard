// fen.cpp

#include "fen.hpp"

std::string FEN::to_string(const Position &position, const Bitboards &board) {
  std::string fen;

  // Iterates the gameboard's squares and  convert each piece to it's FEN
  // notation
  for (int rank = 7; rank >= 0; --rank) {
    int wo_piece = 0;
    for (int file = 0; file < 8; ++file) {
      int i = rank * 8 + file;

      Color piece_color = board.is_occupied(i);
      if (piece_color == Color::NONE) {
        wo_piece++;
      } else {
        if (wo_piece > 0) {
          // Adds the number of consecutive squares without pieces and resets
          // the counter
          fen.append(std::to_string(wo_piece));
          wo_piece = 0;
        }
        PieceType piece_type = board.get_piece_type(piece_color, i);
        if (piece_type == NONE_PIECE)
          continue;
        fen.push_back(FEN::piece_to_symbol(piece_color, piece_type));
      }
    }
    if (wo_piece > 0)
      fen.append(std::to_string(wo_piece));
    if (rank > 0)
      fen.push_back('/');
  }

  if (position.side_to_move == Color::WHITE)
    fen.append(" w ");
  else
    fen.append(" b ");

  fen.append(FEN::castling_rights_to_fen(position.castling_rights));

  fen.push_back(' ');
  if (position.en_passant_square == -1)
    fen.push_back('-');
  else {
    int file = position.en_passant_square % 8;
    int rank = position.en_passant_square / 8;
    fen.push_back('a' + (char)file);
    fen.push_back('1' + (char)rank);
  }

  fen.push_back(' ');
  fen.append(std::to_string(position.halfmove_clock));
  fen.push_back(' ');
  fen.append(std::to_string(position.fullmove_number));

  return fen;
}

void FEN::load(std::string fen, Position &position, Bitboards &board) {

  if (fen == "startpos") {
    fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  }

  board.clear();

  std::istringstream iss(fen);
  std::vector<std::string> parts;

  std::string part;
  iss >> part;

  int square = 56;
  for (char c : part) {
    if (isdigit(c)) {
      square += c - '0';
    } else if (c == '/') {
      square -= 16;
    } else {
      Color color = isupper(c) ? WHITE : BLACK;
      PieceType type = symbol_to_piece(c);
      board.add_piece(color, type, square);
      square++;
    }
  }

  iss >> part;
  if (part == "b")
    position.side_to_move = Color::BLACK;
  else
    position.side_to_move = Color::WHITE;

  iss >> part;
  position.castling_rights = fen_to_castling_rights(part);

  iss >> part;
  if (part == "-")
    position.en_passant_square = -1;
  else {
    int file = part[0] - 'a';
    int rank = part[1] - '1';
    position.en_passant_square = rank * 8 + file;
  }

  iss >> part;
  position.halfmove_clock = std::atoi(part.c_str());
  iss >> part;
  position.fullmove_number = std::atoi(part.c_str());
}

char FEN::piece_to_symbol(const Color piece_color, const PieceType piece_type) {
  char symbol;
  switch (piece_type) {
  case PieceType::PAWN:
    symbol = 'p';
    break;
  case PieceType::BISHOP:
    symbol = 'b';
    break;
  case PieceType::QUEEN:
    symbol = 'q';
    break;
  case PieceType::ROOK:
    symbol = 'r';
    break;
  case PieceType::KNIGHT:
    symbol = 'n';
    break;
  case PieceType::KING:
    symbol = 'k';
    break;
  default:
    symbol = ' ';
    break;
  }
  if (piece_color == Color::WHITE)
    return (char)toupper(symbol);
  else
    return symbol;
}

PieceType FEN::symbol_to_piece(const char piece_symbol) {
  char symbol = (char)std::tolower(piece_symbol);
  switch (symbol) {
  case 'p':
    return PieceType::PAWN;
  case 'b':
    return PieceType::BISHOP;
  case 'q':
    return PieceType::QUEEN;
  case 'r':
    return PieceType::ROOK;
  case 'n':
    return PieceType::KNIGHT;
  case 'k':
    return PieceType::KING;
  default:
    return PieceType::NONE_PIECE;
  }
}

std::string FEN::castling_rights_to_fen(uint8_t rights) {
  std::string rights_str;
  if ((rights >> 0) & 1)
    rights_str.append("K");
  if ((rights >> 1) & 1)
    rights_str.append("Q");
  if ((rights >> 2) & 1)
    rights_str.append("k");
  if ((rights >> 3) & 1)
    rights_str.append("q");
  if (rights_str.empty())
    rights_str.append("-");
  return rights_str;
}

uint8_t FEN::fen_to_castling_rights(const std::string &fen) {
  uint8_t rights = 0;

  for (char c : fen) {
    switch (c) {
    case 'K':
      rights |= (1 << 0);
      break;
    case 'Q':
      rights |= (1 << 1);
      break;
    case 'k':
      rights |= (1 << 2);
      break;
    case 'q':
      rights |= (1 << 3);
      break;
    case '-':
      break;
    default:
      break;
    }
  }

  return rights;
}
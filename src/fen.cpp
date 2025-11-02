// fen.cpp

#include "fen.hpp"


std::string FEN::to_string(const GameState& game, const Bitboards& board) {
    std::string fen;
    
    // Iterates the gameboard's squares and  convert each piece to it's FEN notation
    for (int rank = 7; rank >= 0; --rank) {
        int wo_piece = 0;
        for (int file = 0; file < 8; ++file) {
            int i = rank * 8 + file;

            Color piece_color = board.is_occupied(i);
            if (piece_color == Color::NONE) {
                wo_piece++;
            }
            else {
                if (wo_piece > 0)
                {
                    // Adds the number of consecutive squares without pieces and resets the counter
                    fen.append(std::to_string(wo_piece));
                    wo_piece = 0;
                }
                PieceType piece_type = board.get_piece_type(piece_color, i);
                if (piece_type == NONE_PIECE) continue;
                fen.push_back(FEN::piece_to_symbol(piece_color, piece_type));
            }
        }
        if (wo_piece > 0) fen.append(std::to_string(wo_piece));
        if (rank > 0) fen.push_back('/');  
    }

    if (game.side_to_move == Color::WHITE) fen.append(" w ");
    else fen.append(" b ");

    fen.append(FEN::castling_rights_to_fen(game.castling_rights));
    fen.append(std::to_string(game.fullmove_number / 2));
    fen.push_back(' ');
    fen.append(std::to_string(game.fullmove_number / 2));

    return fen;
}


void FEN::load(std::string fen, GameState& game_state, Bitboards& board) {

    if (fen == "startpos") {
        fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    }

    for (size_t i = 0; i < 63; i++)
    {
        Color color = board.is_occupied(i);
        if (color != Color::NONE) {
            PieceType type = board.get_piece_type(color, i);
            board.remove_piece(color, type, i);
        }
    }

    std::istringstream iss(fen);
    std::vector<std::string> parts;
    
    std::string part;
    iss >> part;

    int counter = 56;
    for (char c : part)
    {
        if (std::isdigit(c)) {
            counter += c - '0';
        }
        else if (c != '/') {
            Color piece_color = (std::isupper(c)) ? Color::WHITE : Color::BLACK;
            board.add_piece(piece_color, FEN::symbol_to_piece(c), counter++);
        }
        else {
            counter -= 16;
        }
    }

    iss >> part;
    if (part == "b") game_state.side_to_move = Color::BLACK;

    iss >> part;
    game_state.castling_rights = fen_to_castling_rights(part);

    iss >> part; // TODO: handle en passant square

    iss >> part;
    game_state.fullmove_number = 0;
    game_state.fullmove_number += std::atoi(part.c_str());
    iss >> part;
    game_state.fullmove_number += std::atoi(part.c_str());
}


char FEN::piece_to_symbol(const Color piece_color, const PieceType piece_type) {
    char symbol;
    switch (piece_type)
    {
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
    if (piece_color == Color::WHITE) return (char)toupper(symbol);
    else return symbol;
}


PieceType FEN::symbol_to_piece(const char piece_symbol) {
    char symbol = std::tolower(piece_symbol);
    switch (symbol)
    {
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


std::string FEN::castling_rights_to_fen(int rights) {
    std::string rights_str = "";
    if ((rights >> 1) & 1) rights_str.append("K");
    if ((rights >> 2) & 1) rights_str.append("Q");
    if ((rights >> 3) & 1) rights_str.append("k");
    if ((rights >> 4) & 1) rights_str.append("q");
    rights_str.append(" - ");
    return rights_str;
}


int FEN::fen_to_castling_rights(const std::string fen) {
    int rights = 0;
    int counter = 1;
    
    if (fen.at(counter) == 'K') {
        rights |= (1 << counter);
        counter++;
    }
    if (fen.at(counter) == 'Q') {
        rights |= (1 << counter);
        counter++;
    }
    if (fen.at(counter) == 'k') {
        rights |= (1 << counter);
        counter++;
    }
    if (fen.at(counter) == 'q') {
        rights |= (1 << counter);
        counter++;
    }
    return rights;
}
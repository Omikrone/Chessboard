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
                fen.push_back(FEN::piece_symbol(piece_color, piece_type));
            }
        }
        if (wo_piece > 0) fen.append(std::to_string(wo_piece));
        if (rank > 0) fen.push_back('/');  
    }

    if (game.side_to_move == Color::WHITE) fen.append(" w ");
    else fen.append(" b ");

    fen.append(FEN::castling_rights(game.castling_rights));
    fen.append(std::to_string(game.fullmove_number / 2));
    fen.push_back(' ');
    fen.append(std::to_string(game.fullmove_number / 2));

    return fen;
}


char FEN::piece_symbol(const Color piece_color, const PieceType piece_type) {
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
    if (piece_color == Color::WHITE) return toupper(symbol);
    else return symbol;
}


std::string FEN::castling_rights(int rights) {
    std::string rights_str = "";
    if ((rights >> 1) & 1) rights_str.append("K");
    if ((rights >> 2) & 1) rights_str.append("Q");
    if ((rights >> 3) & 1) rights_str.append("k");
    if ((rights >> 4) & 1) rights_str.append("q");
    rights_str.append(" - ");
    return rights_str;
}
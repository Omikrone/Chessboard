// move_generator.cpp

#include "game/rules/move_generator.hpp"


std::vector<Move> MoveGenerator::get_all_possible_moves(GameBoard& board, Color side) {
    std::vector<Move> allPossibleMoves;

    // Iterates on all the pieces with color 'side' and generates their moves
    for (auto& rank: board._board) {
        for (const auto& cell: rank) {
            if (cell != nullptr && cell->_color == side) {
                std::vector<Move> possibleMoves = get_possible_moves(board, cell.get());
                allPossibleMoves.insert(allPossibleMoves.end(), possibleMoves.begin(), possibleMoves.end()); // Concate two vectors
            }
        }
    }
    return allPossibleMoves;
}

std::vector<Move> MoveGenerator::get_possible_moves(GameBoard& board, Piece *piece) {

    // TODO: refactor this class and improve Single responsibility
    std::vector<std::vector<Square>> rawMoves = piece->get_raw_moves();
    std::vector<Move> rawPossibleMoves;

    // Handle the special moves (castle, EnPassant ...)
    if (piece->_pieceType == PieceType::PAWN) {

        for (std::vector<Square> dm: rawMoves) {
            if (dm.empty()) continue;

            // Forward move for a pawn: check if there's no obstacle or if a promotion is possible
            if (dm[0].file == piece->_position.file) {
                for (Square p: dm) {
                    Piece *presentPiece = board._board[p.rank][p.file].get();
                    if (presentPiece != nullptr) {
                        break;
                    }
                    else if (p.rank == BOARD_LENGTH - 1 || p.rank == 0) {
                        rawPossibleMoves.push_back({piece->_position, p, MoveType::PROMOTION, false});
                    }
                    else {
                        rawPossibleMoves.push_back({piece->_position, p, MoveType::NORMAL, false});
                    }
                }
            }

            // Side move for a pawn: check if promotion or enpassant is possible
            else {
                Square takePosition = dm[0];
                Piece *presentPiece = board._board[takePosition.rank][takePosition.file].get();
                if (presentPiece != nullptr && presentPiece->_color != piece->_color)
                {
                    if (takePosition.rank == BOARD_LENGTH - 1 || takePosition.rank == 0) {
                        rawPossibleMoves.push_back({piece->_position, takePosition, MoveType::PROMOTION, true});
                    }
                    else {
                        rawPossibleMoves.push_back({piece->_position, takePosition, MoveType::NORMAL, true});
                    }
                }
                else {
                    rawPossibleMoves.push_back({piece->_position, takePosition, MoveType::EN_PASSANT, true});
                }
            }
        }
    }
    else if (piece->_pieceType == PieceType::KING) {
        for (std::vector<Square> dm: rawMoves)
        {
            // Moves for a King: converts raw moves to castle
            for (Square p: dm) {
                int8_t distance  = p.file - piece->_position.file;
                Piece *presentPiece = board._board[p.rank][p.file].get();
                if (distance == 2) rawPossibleMoves.push_back({piece->_position, p, MoveType::CASTLE_KINGSIDE});
                else if (distance == -2) rawPossibleMoves.push_back({piece->_position, p, MoveType::CASTLE_QUEENSIDE});
                else if (presentPiece != nullptr && presentPiece->_color == piece->_color) {
                    break;
                }
                else if (presentPiece != nullptr && presentPiece->_color != piece->_color) {
                    rawPossibleMoves.push_back({piece->_position, p, MoveType::NORMAL, true});
                    break;
                }
                else {
                    rawPossibleMoves.push_back({piece->_position, p, MoveType::NORMAL, false});
                }
            }
        }
    }
    else {

        // For normal moves
        for (std::vector<Square> dm: rawMoves)
        {
            for (Square p: dm) {
                Piece *presentPiece = board._board[p.rank][p.file].get();
                if (presentPiece != nullptr && presentPiece->_color == piece->_color) {
                    break;
                }
                else if (presentPiece != nullptr && presentPiece->_color != piece->_color) {
                    rawPossibleMoves.push_back({piece->_position, p, MoveType::NORMAL, true});
                    break;
                }
                else {
                    rawPossibleMoves.push_back({piece->_position, p, MoveType::NORMAL, false});
                }
            }
        }
    }
    return rawPossibleMoves;
}
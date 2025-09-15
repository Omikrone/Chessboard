// queen.cpp

#include "game/pieces/queen.hpp"


Queen::Queen(PieceType pieceType, const Square &startPosition, Color color)
    : Piece(pieceType, startPosition, color) {}


std::vector<std::vector<Square>> Queen::get_raw_moves() const {

    // The queen movements are the rook moves comined with the bishop moves
    std::vector<std::vector<Square>> possibleMoves = Bishop::get_bishop_moves(_position);
    std::vector<std::vector<Square>> rookMoves = Rook::get_rook_moves(_position);

    possibleMoves.insert(possibleMoves.end(), rookMoves.begin(), rookMoves.end());
    return possibleMoves;
}


std::unique_ptr<Piece> Queen::clone() const {
    return std::make_unique<Queen>(*this);
}

char Queen::to_fen() const {
    if (_color == Color::WHITE) return 'Q';
    else return 'q';
}
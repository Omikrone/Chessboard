// bishop.cpp

#include "game/pieces/bishop.hpp"


Bishop::Bishop(PieceType pieceType, const Square &startPosition, Color color)
    : Piece(pieceType, startPosition, color) {}


std::vector<std::vector<Square>> Bishop::get_raw_moves() const {
    return get_bishop_moves(_position);
}


std::vector<std::vector<Square>> Bishop::get_bishop_moves(const Square &currentPosition) {
    std::vector<std::vector<Square>> possibleMoves;

    int8_t count = 1;
    std::vector<Square> directionMoves;
    while ((currentPosition.file + count) < BOARD_LENGTH && (currentPosition.rank + count) < BOARD_LENGTH)
    {
        directionMoves.push_back({currentPosition.file + count, currentPosition.rank + count});
        count++;
    }
    possibleMoves.push_back(directionMoves);

    count = 1;
    directionMoves.clear();
    while ((currentPosition.file - count) >= 0 && (currentPosition.rank + count) < BOARD_LENGTH)
    {
        directionMoves.push_back({currentPosition.file - count, currentPosition.rank + count});
        count++;
    }
    possibleMoves.push_back(directionMoves);

    count = 1;
    directionMoves.clear();
    while ((currentPosition.file + count) < BOARD_LENGTH && (currentPosition.rank - count) >= 0)
    {
        directionMoves.push_back({currentPosition.file + count, currentPosition.rank - count});
        count++;
    }
    possibleMoves.push_back(directionMoves);

    count = 1;
    directionMoves.clear();
    while ((currentPosition.file - count) >= 0 && (currentPosition.rank - count) >= 0)
    {
        directionMoves.push_back({currentPosition.file - count, currentPosition.rank - count});
        count++;
    }
    possibleMoves.push_back(directionMoves);

    return possibleMoves;
}


std::unique_ptr<Piece> Bishop::clone() const {
    return std::make_unique<Bishop>(*this);
}


char Bishop::to_fen() const {
    if (_color == Color::WHITE) return 'B';
    else return 'b';
}
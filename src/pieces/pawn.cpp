// pawn.cpp

#include "game/pieces/pawn.hpp"


Pawn::Pawn(PieceType pieceType, const Square &startPosition, Color color)
    : Piece(pieceType, startPosition, color) {}


std::vector<std::vector<Square>> Pawn::get_raw_moves() const {
    std::vector<std::vector<Square>> possibleMoves;

    std::int8_t direction = static_cast<std::int8_t>(_color);

    if (_position.rank + direction >= 0 && _position.rank + direction < BOARD_LENGTH) {

        std::vector<Square> directionMoves;

        // Forward walk
        directionMoves.push_back(Square{_position.file, _position.rank + direction});
        if (_color == Color::WHITE && _position.rank == 1 ||
            _color == Color::BLACK && _position.rank == 6) {
                directionMoves.push_back(Square{_position.file, _position.rank + direction * 2});
            }
        possibleMoves.push_back(directionMoves);

        if (_position.file > 0) {
            directionMoves.clear();
            // First side capture
            directionMoves.push_back(Square{_position.file - 1, _position.rank + direction});
            possibleMoves.push_back(directionMoves);
        }
        if (_position.file < BOARD_LENGTH - 1) {
            directionMoves.clear();
            // Second side capture
            directionMoves.push_back(Square{_position.file + 1, _position.rank + direction});
            possibleMoves.push_back(directionMoves);
        }
    }
    return possibleMoves;
}


std::unique_ptr<Piece> Pawn::clone() const {
    return std::make_unique<Pawn>(*this);
}


char Pawn::to_fen() const {
    if (_color == Color::WHITE) return 'P';
    else return 'p';
}
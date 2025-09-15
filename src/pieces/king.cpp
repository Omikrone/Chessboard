// king.cpp

#include "game/pieces/king.hpp"


King::King(PieceType pieceType, const Square &startPosition, Color color)
    : Piece(pieceType, startPosition, color) {}

std::vector<std::vector<Square>> King::get_raw_moves() const {
    std::vector<std::vector<Square>> possibleMoves;
    std::vector<Square> directionMoves;

    for (std::int8_t i = -1; i <= 1; i++)
    {
        for (std::int8_t j = -1; j <= 1; j++)
        {
            if (j == 0 && i == 0) continue;

            int8_t newX = _position.file + i;
            int8_t newY = _position.rank + j;

            if (newX >= 0 && newX < BOARD_LENGTH &&
                newY >= 0 && newY < BOARD_LENGTH) {
                directionMoves.push_back({newX, newY});
                if (i == -1 && j == 0 && _position.file > 3) directionMoves.push_back({newX - 1, newY});                // Queen Side Castle
                if (i == 1 && j == 0 && _position.file < BOARD_LENGTH - 3) directionMoves.push_back({newX + 1, newY});  // King Side Castle
                possibleMoves.push_back(directionMoves);
                directionMoves.clear();
            }
        }
    }
    
    return possibleMoves;
}


std::unique_ptr<Piece> King::clone() const {
    return std::make_unique<King>(*this);
}


char King::to_fen() const {
    if (_color == Color::WHITE) return 'K';
    else return 'k';
}
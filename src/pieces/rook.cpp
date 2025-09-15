// rook.cpp

#pragma once

#include "game/pieces/rook.hpp"


Rook::Rook(PieceType pieceType, const Square &startPosition, Color color)
    : Piece(pieceType, startPosition, color) {}

            
std::vector<std::vector<Square>> Rook::get_raw_moves() const {
    return get_rook_moves(_position);
}

std::vector<std::vector<Square>> Rook::get_rook_moves(const Square &currentPosition) {
    std::vector<std::vector<Square>> possibleMoves;

    // Direction to the top
    std::vector<Square> directionMoves;
    for (std::int8_t i = currentPosition.rank + 1; i < BOARD_LENGTH; i++)
    {
        directionMoves.push_back({currentPosition.file, i});
    }
    possibleMoves.push_back(directionMoves);

    // Direction to the bottom
    directionMoves.clear();
    for (std::int8_t i = currentPosition.rank - 1; i >= 0; i--)
    {
        directionMoves.push_back({currentPosition.file, i});
    }
    possibleMoves.push_back(directionMoves);

    // Direction to the left
    directionMoves.clear();
    for (std::int8_t i = currentPosition.file - 1; i >= 0; i--)
    {
        directionMoves.push_back({i, currentPosition.rank});
    }
    possibleMoves.push_back(directionMoves);

    // Direction to the right
    directionMoves.clear();
    for (std::int8_t i = currentPosition.file + 1; i < BOARD_LENGTH; i++)
    {
        directionMoves.push_back({i, currentPosition.rank});
    }
    possibleMoves.push_back(directionMoves);

    return possibleMoves;
}


std::unique_ptr<Piece> Rook::clone() const {
    return std::make_unique<Rook>(*this);
}
     

char Rook::to_fen() const {
    if (_color == Color::WHITE) return 'R';
    else return 'r';
}
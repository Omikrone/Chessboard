// game.cpp

#include "game/game.hpp"


Game::Game()
    : _board(),
      _history(),
      _moveValidator(_board, _history),
      _currentTurn(Color::WHITE),
      _blackMovesNb(0),
      _whiteMovesNb(0)
{}


bool Game::try_apply_move(const Move& move) {

    Piece *piece = _board.get_piece_at(move.initPos);

    if (piece == nullptr || piece->_color != _currentTurn) return false; // The player can't play a piece from the other side

    // Verifies that the move is legal
    std::vector<Move> legalMoves = get_legal_moves(move.initPos);
    for (Move m: legalMoves) {
        if (m == move) {
            _board.make_move(m);
            _history.push(m);
            return true;
        }
    }
    return false;
}


std::vector<Move> Game::get_legal_moves(const Square sq) {

    Piece *piece = _board.get_piece_at(sq);
    if (piece == nullptr) {
        return std::vector<Move>{};
    }

    // Generates the raw possible moves for and filters the legal moves according to the chess rules
    std::vector<Move> rawMoves = MoveGenerator::get_possible_moves(_board, piece);
    std::vector<Move> legalMoves = _moveValidator.filter_legal_moves(rawMoves, piece->_color);

    return legalMoves;
}


GameState Game::get_game_state() {

    // If the current player has at least one possible moves, the game isn't finished
    for (auto& row: _board._board) {
        for (const auto &cell: row) {
            if (cell != nullptr && cell->_color == _currentTurn) {
                std::vector<Move> possibleMoves = MoveGenerator::get_possible_moves(_board, cell.get());
                std::vector<Move> legalMoves = _moveValidator.filter_legal_moves(possibleMoves, cell.get()->_color);
                if (!legalMoves.empty()) return GameState::CONTINUING;
            }
        }
    }
    
    // Generates the enemy moves to know if the king is in check
    std::vector<Move> enemyMoves;
    if (_currentTurn == Color::WHITE) enemyMoves = MoveGenerator::get_all_possible_moves(_board, Color::BLACK);
    else enemyMoves = MoveGenerator::get_all_possible_moves(_board, Color::WHITE);

    King& king = _board.get_king(_currentTurn);
    // No possible moves left + king in check = checkmate, else stalemate
    if (_board.is_square_attacked(enemyMoves, king._position)) return GameState::CHECKMATE;
    else return GameState::STALEMATE;
}


void Game::next_turn() {
    _currentTurn = (_currentTurn == Color::WHITE) ? Color::BLACK : Color::WHITE;
}


Color Game::get_current_turn() const {
    return _currentTurn;
}


GameBoard& Game::get_game_board() {
    return _board;
}


int Game::get_nb_moves(Color side) const {
    if (side == Color::WHITE) return _whiteMovesNb;
    else return _blackMovesNb;
}
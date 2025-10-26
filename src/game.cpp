// game.cpp

#include "game.hpp"

#include <iostream>


Game::Game()
    :   _board(_game_state),
        _history(),
        _zobrist(_game_state, _board),
        _executor(_history, _game_state, _board, _zobrist),
        _validator(_game_state, _board, _executor)
{}


bool Game::try_apply_move(const int from, const int to) {

    Color piece_color = _board.is_occupied(from);
    if (piece_color != _game_state.side_to_move) return false; // The player can't play a piece from the other side
    PieceType piece_type = _board.get_piece_type(_game_state.side_to_move, from);

    // Verifies that the move is legal
    std::vector<Move> moves = MoveGenerator::piece_moves(from, _game_state.side_to_move, piece_type, _game_state);
    for (Move m: moves) {
        if (m.from == from && m.to == to && _validator.is_legal(m)) {
            _executor.make_move(_game_state.side_to_move, m);
            return true;
        }
    }
    return false;
}


EndGame Game::get_game_state() {

    if (_game_state.halfmove_clock >= 75) return EndGame::STALEMATE;

    bool repetition = true;
    int counter = 0;
    for (int i = 1; i <= _history.size(); i++)
    {
        if (_history.size() < 6) {
            repetition = false;
            break;
        }
        if (_history.at(_history.size() - i).zobrist_hash == _game_state.zobrist_hash) {
            counter++;
        }
    }

    if (counter >= 3) return EndGame::STALEMATE;

    // If the current player has at least one possible moves, the game isn't finished
    std::vector<Move> possible_moves = get_legal_moves(_game_state.side_to_move);
    if (!possible_moves.empty()) return EndGame::CONTINUING;

    // If the king is in check, it's checkmate, else it's stalemate
    if (_validator.is_king_in_check(_game_state.side_to_move)) return EndGame::CHECKMATE;
    else return EndGame::STALEMATE;
}


void Game::next_turn() {
    _game_state.side_to_move = (_game_state.side_to_move == Color::WHITE) ? Color::BLACK : Color::WHITE;
}


Color Game::get_current_turn() const {
    return _game_state.side_to_move;
}


int Game::get_nb_moves(Color side) const {
    if (side == Color::WHITE) return _game_state.fullmove_number / 2;
    else return _game_state.fullmove_number / 2;
}


std::string Game::get_fen() const {
    return FEN::to_string(_game_state, _board);
}


void Game::load_fen(std::string& fen) {
    FEN::load(fen, _game_state, _board);
}


std::vector<Move> Game::get_legal_moves(const Color side) {
    std::vector<Move> legal_moves;

    std::vector<Move> possible_moves = MoveGenerator::all_possible_moves(_game_state.side_to_move, _game_state, _board);
    for (Move m: possible_moves) {
        if (_validator.is_legal(m)) {
            legal_moves.push_back(m);
        }
    }

    return legal_moves;
}


Bitboards& Game::get_board() {
    return _board;
}


void Game::unmake_move() {
    _executor.unmake_last_move();
}
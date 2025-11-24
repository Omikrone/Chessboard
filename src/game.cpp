// game.cpp

#include "game.hpp"


Game::Game()
    :   _board(_position),
        _history(),
        _zobrist(_position, _board),
        _executor(_history, _position, _board, _zobrist),
        _validator(_position, _board, _executor)
{}


bool Game::try_apply_move(const int from, const int to) {

    Color piece_color = _board.is_occupied(from);
    if (piece_color != _position.side_to_move) return false; // The player can't play a piece from the other side
    PieceType piece_type = _board.get_piece_type(_position.side_to_move, from);

    // Verifies that the move is legal
    std::vector<Move> moves = MoveGenerator::piece_moves(from, _position.side_to_move, piece_type, _position);
    for (Move m: moves) {
        if (m.from == from && m.to == to && _validator.is_legal(m)) {
            _executor.make_move(_position.side_to_move, m);
            return true;
        }
    }
    return false;
}


GameState Game::get_game_state() {

    if (_position.halfmove_clock >= 50) return GameState::DRAW_BY_FIFTY_MOVE_RULE;
    if (_position.halfmove_clock >= 75) return GameState::DRAW_BY_75_MOVE_RULE;

    bool repetition = true;
    int counter = 0;
    for (int i = 1; i <= _history.size(); i++)
    {
        if (_history.size() < 6) {
            repetition = false;
            break;
        }
        if (_history.at(_history.size() - i).zobrist_hash == _position.zobrist_hash) {
            counter++;
        }
    }

    if (counter >= 3) return GameState::DRAW_BY_THREEFOLD_REPETITION;

    // If the current player has at least one possible moves, the game isn't finished
    std::vector<Move> possible_moves = get_legal_moves(_position.side_to_move);
    if (!possible_moves.empty()) return GameState::CONTINUING;

    // If the king is in check, it's checkmate, else it's stalemate
    if (_validator.is_king_in_check(_position.side_to_move)) return GameState::CHECKMATE;
    else return GameState::STALEMATE;
}


void Game::next_turn() {
    _position.side_to_move = (_position.side_to_move == Color::WHITE) ? Color::BLACK : Color::WHITE;
}


Color Game::get_current_turn() const {
    return _position.side_to_move;
}


int Game::get_fullmove_number() const {
    return _position.fullmove_number;
}


int Game::get_halfmove_clock() const {
    return _position.halfmove_clock;
}


std::string Game::get_fen() const {
    return FEN::to_string(_position, _board);
}


void Game::load_fen(std::string& fen) {
    FEN::load(fen, _position, _board);
}


std::vector<Move> Game::get_legal_moves(const Color side) {
    std::vector<Move> legal_moves;

    std::vector<Move> possible_moves = MoveGenerator::all_possible_moves(_position.side_to_move, _position, _board);
    for (Move m: possible_moves) {
        if (_validator.is_legal(m)) {
            legal_moves.push_back(m);
        }
    }

    return legal_moves;
}

std::vector<Move> Game::get_played_moves() const {
    return _history.get_moves();
}

Bitboards& Game::get_board() {
    return _board;
}


void Game::unmake_move() {
    _executor.unmake_last_move();
}
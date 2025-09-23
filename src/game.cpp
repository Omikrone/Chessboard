// game.cpp

#include "game.hpp"


Game::Game()
    :   _board(_game_state),
        _history(),
        _executor(_history, _game_state, _board),
        _validator(_game_state, _board, _executor)
      
{}


bool Game::try_apply_move(const int from, const int to) {

    Color piece_color = _board.is_occupied(from);
    if (piece_color != _game_state.side_to_move) return false; // The player can't play a piece from the other side
    PieceType piece_type = _board.get_piece_type(_game_state.side_to_move, from);

    // Verifies that the move is legal
    std::vector<Move> moves = MoveGenerator::piece_moves(from, _game_state.side_to_move, piece_type, _game_state);
    for (Move m: moves) {
        if (_validator.is_legal(m)) {
            _executor.make_move(_game_state.side_to_move, m);
            _history.push(_game_state);
            return true;
        }
    }
    return false;
}


EndGame Game::get_game_state() {

    // If the current player has at least one possible moves, the game isn't finished
    for (int i=0; i < 64; i++) {
        std::vector<Move> possible_moves = MoveGenerator::all_possible_moves(_game_state.side_to_move, _game_state, _board);

        for (Move m: possible_moves) {
            if (_validator.is_legal(m)) return EndGame::CONTINUING;
        }

        if (possible_moves.empty()) return EndGame::STALEMATE;
        else return EndGame::CHECKMATE;
    }
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
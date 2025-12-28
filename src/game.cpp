// game.cpp

#include "game.hpp"

Game::Game()
    : _board(_position),
      _history(),
      _zobrist(_position, _board),
      _generator(_position, _board),
      _executor(_history, _position, _board, _zobrist),
      _validator(_position, _board, _executor, _generator) {}

bool Game::try_apply_move(Move const &move) {
    Color piece_color = _board.is_occupied(move.from);
    if (piece_color != _position.side_to_move) return false;  // The player can't play a piece from the other side
    PieceType piece_type = _board.get_piece_type(_position.side_to_move, move.from);
    if (move.from < 0 || move.from >= 64 || move.to < 0 || move.to >= 64) {
        std::cerr << "Error: Invalid move coordinates (" << move.from << " to " << move.to << ")." << std::endl;
        std::cerr << "Piece color: " << static_cast<int>(piece_color) << ", piece type: " << static_cast<int>(piece_type) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Verifies that the move is legal
    std::vector<Move> moves = _generator.piece_moves(move.from, _position.side_to_move, piece_type);
    for (Move m : moves) {
        if (m.from == move.from && m.to == move.to && _validator.is_legal(m)) {
            _executor.make_move(_position.side_to_move, m, move.promotion_type);
            next_turn();
            return true;
        }
    }
    return false;
}

GameState Game::get_game_state() {
    if (_position.halfmove_clock >= 50) return GameState::DRAW_BY_FIFTY_MOVE_RULE;
    if (_position.halfmove_clock >= 75) return GameState::DRAW_BY_75_MOVE_RULE;

    int counter = 0;
    for (size_t i = 1; i <= _history.size(); i++) {
        if (_history.size() < 6) {
            break;
        }
        if (_history.at(_history.size() - i).zobrist_hash == _position.zobrist_hash) {
            counter++;
        }
    }

    if (counter >= 3) return GameState::DRAW_BY_THREEFOLD_REPETITION;

    // If the current player has at least one possible moves, the game isn't
    // finished
    std::vector<Move> possible_moves = get_legal_moves();
    if (!possible_moves.empty()) return GameState::CONTINUING;

    // If the king is in check, it's checkmate, else it's stalemate
    if (_validator.is_king_in_check(_position.side_to_move))
        return GameState::CHECKMATE;
    else
        return GameState::STALEMATE;
}

void Game::next_turn() {
    _position.side_to_move = (_position.side_to_move == Color::WHITE) ? Color::BLACK : Color::WHITE;
}

Color Game::get_current_turn() const { return _position.side_to_move; }

int Game::get_fullmove_number() const { return _position.fullmove_number; }

int Game::get_halfmove_clock() const { return _position.halfmove_clock; }

std::string Game::get_fen() const { return FEN::to_string(_position, _board); }

void Game::load_fen(std::string &fen) { FEN::load(fen, _position, _board); }

std::vector<Move> Game::get_legal_moves() {
    std::vector<Move> legal_moves;

    std::vector<Move> possible_moves = _generator.all_possible_moves(_position.side_to_move);
    for (Move m : possible_moves) {
        if (_validator.is_legal(m)) {
            legal_moves.push_back(m);
        }
    }

    return legal_moves;
}

std::vector<Move> Game::get_capture_and_check_moves() {
    std::vector<Move> capture_and_check_moves;
    
    std::vector<Move> possible_moves = _generator.all_possible_moves(_position.side_to_move);
    for (Move m : possible_moves) {
        auto [legal, gives_check] = _validator.is_legal_and_check(m);
        if (legal && (m.take || gives_check)) {
            capture_and_check_moves.push_back(m);
        }
    }
    
    return capture_and_check_moves;
}

std::vector<Move> Game::get_played_moves() const { return _history.get_moves(); }

Bitboards &Game::get_board() { return _board; }

void Game::unmake_move() { _executor.unmake_last_move(); }
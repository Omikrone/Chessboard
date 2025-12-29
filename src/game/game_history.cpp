#include "game_history.hpp"

GameHistory::GameHistory() {
    Move first_move = {0, 0, MoveType::NORMAL, false};
    UndoMove undo = {first_move, Color::WHITE, UINT8_MAX, -1, 0, 0};
    push(undo);
}

void GameHistory::push(UndoMove undo) { _history.push_back(undo); }

UndoMove GameHistory::pop() {
    UndoMove undo = _history.back();
    _history.pop_back();
    return undo;
}

UndoMove GameHistory::last() const {
    assert(!_history.empty());
    return _history.back();
}

bool GameHistory::empty() const { return _history.empty(); }

size_t GameHistory::size() const { return _history.size(); }

UndoMove GameHistory::at(size_t index) const { return _history.at(index); }

std::vector<Move> GameHistory::get_moves() const {
    std::vector<Move> moves;
    for (size_t i = 1; i < _history.size(); i++) {
        moves.push_back(_history[i].move);
    }
    return moves;
}
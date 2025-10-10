// game_history.cpp

#include "game_history.hpp"


GameHistory::GameHistory(UndoMove game_state) {
    push(game_state);
}

void GameHistory::push(UndoMove game_state) {
    _history.push_back(game_state);
}

UndoMove GameHistory::pop() {
    UndoMove undo = _history.back();
    _history.pop_back();
    return undo;
}

UndoMove GameHistory::last() const {
    assert(!_history.empty());
    return _history.back();
}

bool GameHistory::empty() const {
    return _history.empty();
}

size_t GameHistory::size() const {
    return _history.size();
}
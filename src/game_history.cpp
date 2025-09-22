// game_history.cpp

#include "game_history.hpp"


void GameHistory::push(const GameState game_state) {
    _history.push_back(game_state);
}

void GameHistory::pop() {
    return _history.pop_back();
}

const GameState& GameHistory::last() const {
    assert(!_history.empty());
    return _history.back();
}

bool GameHistory::empty() const {
    return _history.empty();
}
// game_history.cpp

#include "game_history.hpp"


GameHistory::GameHistory(GameState game_state) {
    push(game_state);
}

void GameHistory::push(GameState game_state) {
    _history.push_back(game_state);
}

void GameHistory::pop() {
    return _history.pop_back();
}

GameState GameHistory::last() const {
    assert(!_history.empty());
    return _history.back();
}

bool GameHistory::empty() const {
    return _history.empty();
}

size_t GameHistory::size() const {
    return _history.size();
}
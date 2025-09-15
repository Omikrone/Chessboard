// game_history.cpp

#include "game/game_history.hpp"


void GameHistory::push(Move move) {
    _history.push_back(move);
}

const Move& GameHistory::last() const {
    assert(!_history.empty());
    return _history.back();
}

bool GameHistory::empty() const {
    return _history.empty();
}
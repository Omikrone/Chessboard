// move_executor.hpp

#pragma once

#include "bitboards.hpp"
#include "components/move.hpp"


class MoveExecutor
{
public:
    static void make_move(GameState& game_state, const Move& move);
    static void unmake_move(GameState& game_state, const Move& move);
};
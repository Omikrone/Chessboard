// move_validator.hpp

#pragma once

#include "bitboards.hpp"
#include "components/move.hpp"
#include "rules/move_generator.hpp"
#include "rules/move_executor.hpp"

#include <bit>
#include <vector>


class MoveValidator
{
private:
    const GameState& _game_state;
    const Bitboards& _board;
    MoveExecutor& _executor;

    bool is_square_attacked(const int square, const Color opponent) const;
    bool is_king_in_check(const Color king_color) const;

public:
    MoveValidator(const GameState& position, const Bitboards& board, MoveExecutor& executor);
    ~MoveValidator() = default;

    bool is_legal(const Move& move);
};
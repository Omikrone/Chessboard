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
    GameState& _game_state;
    Bitboards& _board;
    MoveExecutor& _executor;

    bool is_square_attacked(const int square, const Color opponent) const;
    bool is_king_in_check(const Color king_color) const;
    bool check_castle(const Move& move, const Color king_color) const;
    int find_king(const Color king_color) const;

public:
    MoveValidator(GameState& position, Bitboards& board, MoveExecutor& executor);
    ~MoveValidator() = default;

    bool is_legal(const Move& move);
};
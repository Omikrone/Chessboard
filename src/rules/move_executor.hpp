// move_executor.hpp

#pragma once

#include "bitboards.hpp"
#include "components/move.hpp"
#include "game_history.hpp"


class MoveExecutor
{
    private:
        GameHistory& _history;
        GameState& _game_state;
        Bitboards& _board;

        void normal(const Color side, const Move& move);
        void castle_kingside(const Color side, const Move& move);
        void castle_queenside(const Color side, const Move& move);
        void en_passant(const Color side, const Move& move);
        void promotion(const Color side, const uint8_t square, const PieceType new_piece);

    public:
        MoveExecutor(GameHistory& history, GameState& state, Bitboards& board);
        ~MoveExecutor() = default;
        void make_move(const Color side, const Move& move);
        void unmake_last_move();
};
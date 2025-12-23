#include "tables.hpp"

uint64_t ChessTables::knight_attacks[64];
uint64_t ChessTables::king_attacks[64];

void ChessTables::init_knight_table() {
    const int dx[] = {2, 1, -1, -2, -2, -1, 1, 2};
    const int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};

    for (int s=0; s < 64; s++) {
        knight_attacks[s] = 0ULL;

        int fromX = s % 8;
        int fromY = s / 8;

        for (int i = 0; i < 8; i++) {
            int toX = fromX + dx[i];
            int toY = fromY + dy[i];

            if (toX < 0 || toX > 7 || toY < 0 || toY > 7) continue;

            int to = toY * 8 + toX;
            uint64_t mask = 1ULL << to;
            knight_attacks[s] |= mask;
        } 
    }
}

void ChessTables::init_king_table() {

    for (int s = 0; s < 64; s++) {
        int fromX = s % 8;
        int fromY = s / 8;

        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                if (x == 0 && y == 0) continue;
                int toX = fromX + x;
                int toY = fromY + y;

                if (toX < 0 || toX > 7 || toY < 0 || toY > 7) continue;
                int to = s + x + y * 8;

                uint64_t mask = 1ULL << to;
                king_attacks[s] |= mask;
            }
        }
    }
}
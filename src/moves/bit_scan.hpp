#pragma once

#include <cstdint>

#if defined(__GNUC__) || defined(__clang__)
inline int bitscan_forward(uint64_t bb) {
    if (bb == 0) return -1;
    return __builtin_ctzll(bb);
}
#elif defined(_MSC_VER)
#include <intrin.h>
inline int bitscan_forward(uint64_t bb) {
    if (bb == 0) return -1;
    unsigned long index;
    _BitScanForward64(&index, bb);
    return (int)index;
}
#else
inline int bitscan_forward(uint64_t bb) {
    if (bb == 0) return -1;

    static const int index64[64] = {
         0,  1, 48,  2, 57, 49, 28,  3,
        61, 58, 50, 42, 38, 29, 17,  4,
        62, 55, 59, 36, 53, 51, 43, 22,
        45, 39, 33, 30, 24, 18, 12,  5,
        63, 47, 56, 27, 60, 41, 37, 16,
        54, 35, 52, 21, 44, 32, 23, 11,
        46, 26, 40, 15, 34, 20, 31, 10,
        25, 14, 19,  9, 13,  8,  7,  6
    };
    
    const uint64_t debruijn64 = 0x03f79d71b4cb0a89ULL;
    return index64[((bb & -bb) * debruijn64) >> 58];
}
#endif
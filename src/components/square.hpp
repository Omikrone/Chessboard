// square.hpp

#pragma once

#include <cstdint>
#include <iostream>


/**
 * @brief Represents a square on the chess board.
 */
struct Square
{
    int file; // file from 0 ('a') to 7 ('h')
    int rank; // rank from 0 to 7

    bool operator==(const Square &other) const {
        return file == other.file && rank == other.rank;
    }

    bool operator!=(const Square& other) const {
        return !(*this == other);
    }

    void print() const {
        std::cout << "{" << static_cast<int>(file) << ", " << static_cast<int>(rank) << "}";
    }
};
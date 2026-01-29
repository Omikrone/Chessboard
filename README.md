# Chess Board - Optimized C++ Chess Game


## Introduction

This project is a chess library written in C++ and optimized for performance with bitboards. It is intended to be the main library of the (chess game)[https://github.com/Omikrone/Chessgame], and the chess engine (Euphron)[https://github.com/Omikrone/Euphron]. It is designed to be as fast as possible, and has achieved the following performance metrics compared to a naive implementation (the old one):
- **Moves generation and application:** ~60 microseconds (naive: ~2000 microseconds)

Currently, the library is not optimized as much as it could be, but it is already significantly faster than a naive implementation.

- **Current version:** 1.5.1


## Features

This chess library includes the following features:
- Basic chess rules implementation.
- Move generation and application using bitboards for efficiency.
- FEN loading and dumping for easy game state representation.
- A public API for compatibility with other chess engines and interfaces.
- Cancellation of moves for chess engine implementations.
- Pre-computed move tables for king, knight, and pawn to optimize move generation.
- Zobrist hashing for threefold repetition and draw detection.
- UCI format move parsing.


## Installation (with CMake)

For simplicity, if you want to use this library in your project, you can simply add the dependency in your CMakeLists.txt file as follows:

```cmake
    include(FetchContent)

    FetchContent_Declare(
        Chessboard
        GIT_REPOSITORY
        https://github.com/Omikrone/Chessboard.git
        GIT_TAG
        v1.5.0
    )

    FetchContent_MakeAvailable(Chessboard)
    target_link_libraries(your_target PRIVATE Chessboard::Chessboard)
```

You can then include the Chessboard headers in your source files:

```cpp
#include <game.hpp>
```

### Installation (from source)

If you prefer to build the library from source, follow the steps below.

### Prerequisites

Make sure you have the following dependencies installed:
- CMake (see [CMake installation guide](https://cmake.org/install/))
- A C++ compiler (e.g., g++)

### Building the Project

1. Clone the repository:
```bash
    git clone https://github.com/Omikrone/Chessboard.git
    cd Chessboard
```

2. Build the Chessboard library using CMake:
```bash
    cmake -S . -B build
    cmake --build build --config Release
```

## Roadmap

Future improvements for this project will include:
- Implement **Magic Bitboards** for sliding pieces (rook, bishop, queen) to further optimize move generation.
- Optimizing the move generation & application further.
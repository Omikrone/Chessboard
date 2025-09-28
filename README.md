# Chess Board - Optimized C++ Chess Game


## Introduction

- **Current version:** 1.0.0

This project is a simple chess game implementation in C++, optimized for performance with bitboards. It is intended to be the main library of the (chess game)[https://github.com/Omikrone/Chessgame], and the future chess engine *Euphron*. It is designed to be as fast as possible, and has achieved the following performance metrics compared to a naive implementation (the old one):
- **Move generation and application:** ~60 microseconds (naive: ~2000 microseconds)


## Features

This chess game includes the following features:
- Basic chess rules implemented.
- Move generation and application using bitboards for efficiency.
- FEN conversion for easy game state representation.
- A public API for compatibility with other chess engines and interfaces.


## Installation

This project is compatible with Windows and Linux, and requires CMake for building.


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

2. Build the C++ server using CMake:
```bash
    cmake -S . -B build
    cd build && cmake --build . --config Release
```


### Adding the library to the chessgame project

Follow the steps described in the [chessgame repository](https://github.com/Omikrone/Chessgame) to integrate the Chessboard library.


## Future Improvements

Future improvements for this project will include:
- [ ] Fixing the issue of non-detection of stale mate.
- [ ] Adding zobrist hashing for detecting threefold repetition and other advanced rules.
- [ ] Optimizing the move generation further.
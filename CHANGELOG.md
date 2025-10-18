# Change Log
All notable changes to this project will be documented in this file.


## [1.2.0] - 2025-10-18

### Added
- Public function to cancel the last move made.
- Public function to get the board.

### Changed
- Optimization of move cancellation in the Chessboard library.

### Fixed
- Issue with stale mate detection.
- Issue with move cancellation not restoring the previous game state correctly.


## [1.1.0] - 2025-10-05

### Added
- Public function to get the list of legal moves for the current player.

### Fixed
- Ghost castle through other pieces.
- History corruption when cancelling moves.


## [1.0.0] - 2025-09-28

### Added
- Initial release of the Chessboard library.
- Basic chess rules implemented.
- Move generation and application using bitboards for efficiency.
- Cancellation of moves for chess engine implementations.
- FEN conversion for easy game state representation.
- Public API for compatibility with other chess engines and interfaces.
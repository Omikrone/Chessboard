// board.cpp

#include "game/board/board.hpp"


GameBoard::GameBoard()
{
    // Initializes the game board with the pieces
    // TODO: simplify the method or create another class responsible for this

    _board.resize(BOARD_LENGTH); 
    for (auto &row : _board) {
        row.resize(BOARD_LENGTH);
    }

    // Pawns initialization
    for (int8_t i = 0; i < BOARD_LENGTH; i++)
    {
        _board[1][i] = std::make_unique<Pawn>(PieceType::PAWN, Square{i, 1}, Color::WHITE);
    }
    for (int8_t i = 0; i < BOARD_LENGTH; i++)
    {
        _board[6][i] = std::make_unique<Pawn>(PieceType::PAWN, Square{i, 6}, Color::BLACK);
    }

    // Knights initialization
    const Square knightPositions[] = {{1, 7}, {6, 7}, {6, 0}, {1, 0}};
    for (int8_t i = 0; i < 4; i++) {
        Square startPosition = {knightPositions[i].file, knightPositions[i].rank};
        if (i < 2) {
            _board[startPosition.rank][startPosition.file] = std::make_unique<Knight>(PieceType::KNIGHT, startPosition, Color::BLACK);
        }
        else {
            _board[startPosition.rank][startPosition.file] = std::make_unique<Knight>(PieceType::KNIGHT, startPosition, Color::WHITE);
        }
    }

    // Bishops initialization
    const Square bishopPositions[] = {{2, 7}, {5, 7}, {5, 0}, {2, 0}};
    for (int8_t i = 0; i < 4; i++) {
        Square startPosition = {bishopPositions[i].file, bishopPositions[i].rank};
        if (i < 2) {
            _board[startPosition.rank][startPosition.file] = std::make_unique<Bishop>(PieceType::BISHOP, startPosition, Color::BLACK);
        }
        else {
            _board[startPosition.rank][startPosition.file] = std::make_unique<Bishop>(PieceType::BISHOP, startPosition, Color::WHITE);
        }
    }

    // Rooks initialization
    const Square rookPositions[] = {{0, 7}, {7, 7}, {7, 0}, {0, 0}};
    for (int8_t i = 0; i < 4; i++) {
        Square startPosition = {rookPositions[i].file, rookPositions[i].rank};
        if (i < 2) {
            _board[startPosition.rank][startPosition.file] = std::make_unique<Rook>(PieceType::ROOK, startPosition, Color::BLACK);
        }
        else {
            _board[startPosition.rank][startPosition.file] = std::make_unique<Rook>(PieceType::ROOK, startPosition, Color::WHITE);
        }
    }
    
    // Queens initialization
    _board[0][3] = std::make_unique<Queen>(PieceType::QUEEN, Square{3, 0}, Color::WHITE);
    _board[7][3] = std::make_unique<Queen>(PieceType::QUEEN, Square{3, 7}, Color::BLACK);

    // Kings initialization
    _board[0][4] = std::make_unique<King>(PieceType::KING, Square{4, 0}, Color::WHITE);
    _board[7][4] = std::make_unique<King>(PieceType::KING, Square{4, 7}, Color::BLACK);
    _whiteKing = static_cast<King *>(_board[0][4].get());
    _blackKing = static_cast<King *>(_board[7][4].get());
}


Piece *GameBoard::get_piece_at(Square sq) const {
    return _board[sq.rank][sq.file].get();
}


void GameBoard::make_move(const Move& move) {
    Piece *piece = get_piece_at(move.initPos);

    switch (move.type)
    {
        case MoveType::CASTLE_KINGSIDE:
            kingside_castle(piece);
            break;

        case MoveType::CASTLE_QUEENSIDE:
            queenside_castle(piece);
            break;

        case MoveType::EN_PASSANT:
            enpassant(piece, move);
            break;

        case MoveType::PROMOTION:
            // TODO: Make othe promotion type (rook, ...) possible
            promotion(piece, PieceType::QUEEN);
            piece = get_piece_at(move.initPos);
            move_piece(move.initPos, move.destPos);
            
        default:
            // Normal move
            move_piece(move.initPos, move.destPos);
            break;
    }
    piece->_hasMoved = true;
}


void GameBoard::enpassant(Piece *pawn, Move move) {

    // Pawn displacement
    move_piece(move.initPos, move.destPos);

    // Other pawn destruction
    if (pawn->_color == Color::WHITE) {
        _board[move.destPos.rank - 1][move.destPos.file].reset();
    }
    else {
        _board[move.destPos.rank + 1][move.destPos.file].reset();
    }
    pawn->_position = {move.destPos.file, move.destPos.rank};
}


void GameBoard::kingside_castle(Piece *king) {

    // Rook displacement
    Square initPos = {static_cast<int8_t>(king->_position.file + 3), king->_position.rank};
    Square destPos = {king->_position.file + 1, king->_position.rank};
    move_piece(initPos, destPos);

    // King displacement
    destPos = {static_cast<int8_t>(king->_position.file + 2), king->_position.rank};
    move_piece(king->_position, destPos);
}


void GameBoard::queenside_castle(Piece *king) {

    // Rook displacement
    Square initPos = {static_cast<int8_t>(king->_position.file - 4), king->_position.rank};
    Square destPos = {king->_position.file - 1, king->_position.rank};
    move_piece(initPos, destPos);

    // King displacement
    destPos = {static_cast<int8_t>(king->_position.file - 2), king->_position.rank};
    move_piece(king->_position, destPos);
}


void GameBoard::promotion(Piece *pawnToPromote, PieceType piecePieceType) {

    // Stores the pawn promotion data
    Color promotionColor = pawnToPromote->_color;
    Square promotionSq = pawnToPromote->_position;
    std::unique_ptr<Piece> newPiece;

    // Fill the free memory space with the new promotion piece
    switch (piecePieceType)
    {
        case PieceType::ROOK:
            newPiece = std::make_unique<Rook>(piecePieceType, promotionSq, promotionColor);
            break;

        case PieceType::BISHOP:
            newPiece = std::make_unique<Bishop>(piecePieceType, promotionSq, promotionColor);
            break;

        case PieceType::KNIGHT:
            newPiece = std::make_unique<Knight>(piecePieceType, promotionSq, promotionColor);
            break;
        
        default:
            newPiece = std::make_unique<Queen>(piecePieceType, promotionSq, promotionColor);
            break;
    }
    _board[promotionSq.rank][promotionSq.file] = std::move(newPiece);
    print_board();

}


bool GameBoard::is_square_attacked(std::vector<Move>& ennemyMoves, Square sq) {
    for (Move m: ennemyMoves)
    {
        // If a possible (legal) ennemy move reach the square, then the square is attacked by the other team
        if (m.destPos == sq) {
            return true;
        }
    }
    return false;
}


void GameBoard::move_piece(Square from, Square to) {
        
    // Piece displacement
    auto& src = _board[from.rank][from.file];
    if (!src) return;

    std::unique_ptr<Piece> moving = std::move(src);
    _board[to.rank][to.file] = std::move(moving);
    _board[to.rank][to.file]->_position = to;
}


King &GameBoard::get_king(Color kingColor) {
    if (kingColor == Color::WHITE) return *_whiteKing;
    else return *_blackKing;
}


std::unique_ptr<GameBoard> GameBoard::clone() const {
    // Creates another blank game board
    std::unique_ptr<GameBoard> newBoard = std::make_unique<GameBoard>();
    newBoard->_board.resize(BOARD_LENGTH);

    // Iterates over the original game board and copy each piece to the new board
    for (int r = 0; r < BOARD_LENGTH; ++r) {
        newBoard->_board[r].resize(BOARD_LENGTH);
        for (int f = 0; f < BOARD_LENGTH; ++f) {

            if (_board[r][f] != nullptr) {

                // Create a deep copy of the piece
                newBoard->_board[r][f] = _board[r][f]->clone();

                // Gets a pointer for each King to memorize their position
                if (_board[r][f]->_pieceType == PieceType::KING && _board[r][f]->_color == Color::WHITE) {
                    newBoard->_whiteKing = static_cast<King *>(newBoard->_board[r][f].get());
                } 
                else if (_board[r][f]->_pieceType == PieceType::KING && _board[r][f]->_color == Color::BLACK) {
                    newBoard->_blackKing = static_cast<King *>(newBoard->_board[r][f].get());
                } 
            }
            else newBoard->_board[r][f] = nullptr;
        }
    }
    return newBoard;
}


void GameBoard::print_board() {
    for (int8_t j = BOARD_LENGTH - 1; j >= 0; j--)
    {
        for (int8_t i = 0; i < BOARD_LENGTH; i++)
        {
            if (_board[j][i] != nullptr) {
                std::cout << " " << _board[j][i].get()->to_fen() << " ";
            }
            else {
                std::cout << "   ";
            }
        }
        std::cout << std::endl;
    }
}
// board.cpp

#include "board/board.hpp"


GameBoard::GameBoard()
{
    // Static initialization of white pieces
    _white_pawns   = 0x000000000000FF00ULL;
    _white_knights = 0x0000000000000042ULL;
    _white_bishops = 0x0000000000000024ULL;
    _white_rooks   = 0x0000000000000081ULL;
    _white_queens   = 0x0000000000000008ULL;
    _white_king    = 0x0000000000000010ULL;

    // Static initialization of black pieces
    _black_pawns   = 0x00FF000000000000ULL;
    _black_knights = 0x4200000000000000ULL;
    _black_bishops = 0x2400000000000000ULL;
    _black_rooks   = 0x8100000000000000ULL;
    _black_queens   = 0x0800000000000000ULL;
    _black_king = 0x1000000000000000ULL;

    // Merge all pieces bitboards to constitue a color bitboard
    _all_white_pieces = _white_pawns | _white_knights | _white_bishops | _white_rooks | _white_queens | _white_king;
    _all_black_pieces = _black_pawns | _black_knights | _black_bishops | _black_rooks | _black_rooks | _black_king;

    _all_pieces = _all_white_pieces | _all_black_pieces;
}


Color GameBoard::get_color_at(Square sq) const {
    if ((_all_white_pieces >> (sq.rank * 8 + sq.file)) & 1ULL) return Color::WHITE;
    else return Color::BLACK;
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
            // TODO: Make other promotion type (rook, ...) possible
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
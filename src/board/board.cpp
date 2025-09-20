// board.cpp

#include "board/board.hpp"


GameBoard::GameBoard()
{

}


const uint64_t& GameBoard::get_piece_type_board(const PieceType piece_type, Color side) const {
    return _position[side][piece_type];
}


const GameBoard::PieceType get_piece_type_at(Square at, Color side) const {
    for (uint64_t& piece_bb: _position[side])
    {
        if (piece_bb[])
    }
    
}


Color GameBoard::get_color_at(int8_t index) const {
    if ((_all_white_pieces >> index) & 1ULL) return Color::WHITE;
    else return Color::BLACK;
}


void GameBoard::move_piece(Square from, Square to) {


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
    pawn->_position.pieces[Color::WHITE][PieceType::] = {move.destPos.file, move.destPos.rank};
}


void GameBoard::kingside_castle(Piece *king) {

    // Rook displacement
    Square initPos = {static_cast<int8_t>(king->_position.pieces[Color::WHITE][PieceType::].file + 3), king->_position.pieces[Color::WHITE][PieceType::].rank};
    Square destPos = {king->_position.pieces[Color::WHITE][PieceType::].file + 1, king->_position.pieces[Color::WHITE][PieceType::].rank};
    move_piece(initPos, destPos);

    // King displacement
    destPos = {static_cast<int8_t>(king->_position.pieces[Color::WHITE][PieceType::].file + 2), king->_position.pieces[Color::WHITE][PieceType::].rank};
    move_piece(king->_position.pieces[Color::WHITE][PieceType::], destPos);
}


void GameBoard::queenside_castle(Piece *king) {

    // Rook displacement
    Square initPos = {static_cast<int8_t>(king->_position.pieces[Color::WHITE][PieceType::].file - 4), king->_position.pieces[Color::WHITE][PieceType::].rank};
    Square destPos = {king->_position.pieces[Color::WHITE][PieceType::].file - 1, king->_position.pieces[Color::WHITE][PieceType::].rank};
    move_piece(initPos, destPos);

    // King displacement
    destPos = {static_cast<int8_t>(king->_position.pieces[Color::WHITE][PieceType::].file - 2), king->_position.pieces[Color::WHITE][PieceType::].rank};
    move_piece(king->_position.pieces[Color::WHITE][PieceType::], destPos);
}


void GameBoard::promotion(Piece *pawnToPromote, PieceType piecePieceType) {

    // Stores the pawn promotion data
    Color promotionColor = pawnToPromote->_color;
    Square promotionSq = pawnToPromote->_position.pieces[Color::WHITE][PieceType::];
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
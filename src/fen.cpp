// fen.cpp

#include "game/fen.hpp"


std::string FEN::to_string(Game& game) {
    std::string fen;

    // Iterates the gameboard's squares and  convert each piece to it's FEN notation
    GameBoard& board = game.get_game_board();
    for (int8_t i=BOARD_LENGTH - 1; i >= 0; i--) {
        int8_t wo_piece = 0;
        for (int8_t j=0; j < BOARD_LENGTH; j++) {
            if (board._board[i][j].get() != nullptr) {
                if (wo_piece != 0) {
                    // Adds the number of consecutive squares without pieces and resets the counter
                    fen.append(std::to_string(wo_piece));
                    wo_piece = 0;
                }
                fen.push_back(board._board[i][j]->to_fen());
            }
            else {
                wo_piece++;
            }
        }
        // Adds the number of consecutive squares without pieces for a new range
        if (wo_piece != 0) fen.append(std::to_string(wo_piece));
        fen.push_back('/');
    }
    fen.pop_back(); // Remove the last '/'

    if (game.get_current_turn() == Color::WHITE) fen.append(" w ");
    else fen.append(" b ");

    // TODO: Update dynamically the catsle rights of the game 
    fen.append("KQkq - ");
    fen.append(std::to_string(game.get_nb_moves(Color::BLACK)));
    fen.push_back(' ');
    fen.append(std::to_string(game.get_nb_moves(Color::WHITE)));

    return fen;
}
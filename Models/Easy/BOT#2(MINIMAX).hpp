// Piotr Stachowicz
#ifndef CHESS_BOT_2_MINIMAX_HPP
#define CHESS_BOT_2_MINIMAX_HPP

#include "../../Settings/Settings.hpp"
#include "../../Objects/Move/Move.hpp"
#include "../../Objects/Board/Board.hpp"
#include "../../Objects/Piece/Piece.hpp"
#include "../Minimax.hpp"

class MinimaxBot {
public:
    // Settings
    int max_depth = 3;


public:

    Move generate_move(Board &board) {
        Move best_move = minimax(max_depth, board, board.turn, -INT_MAX, INT_MAX).first;
        std::vector<Move> legal_moves = Move::generate_legal_moves(board);
        board.make_move(best_move, legal_moves);
        return best_move;
    }

};


#endif //CHESS_BOT_2_MINIMAX_HPP
#ifndef CHESS_MINIMAX_HPP
#define CHESS_MINIMAX_HPP

#include "../Settings/Settings.hpp"
#include "../Objects/Move/Move.hpp"
#include "../Objects/Board/Board.hpp"
#include "../Objects/Piece/Piece.hpp"

const int pawn_value = 100;
const int bishop_value = 300;
const int knight_value = 300;
const int rook_value = 500;
const int queen_value = 900;

std::pair<Move,int> maxi(int depth, Board& board, bool color);
std::pair<Move,int> mini(int depth, Board& board, bool color);


int evaluate_position(Board& board, bool maximizing) {
    int white_material = 0;
    int black_material = 0;

    for(Piece& piece : board.pieces) {
        if(piece.alive) {
            switch (piece.color) {
                case true:  // White pieces
                    switch (piece.type) {
                        case 1: white_material += pawn_value; break;
                        case 2: white_material += bishop_value; break;
                        case 3: white_material += knight_value; break;
                        case 4: white_material += rook_value; break;
                        case 5: white_material += queen_value; break;
                    }
                    break;
                case false:  // Black pieces
                    switch (piece.type) {
                        case 1: black_material += pawn_value; break;
                        case 2: black_material += bishop_value; break;
                        case 3: black_material += knight_value; break;
                        case 4: black_material += rook_value; break;
                        case 5: black_material += queen_value; break;
                    }
                    break;
            }
        }
    }

    int advantage = white_material - black_material;

    return maximizing ? advantage : -advantage;
}

std::pair<Move,int> minimax(int depth, Board& board, bool maximizing) {
    if (depth == 0) return {Move(), evaluate_position(board, maximizing)};

    std::vector<Move> possible_moves = Move::generate_legal_moves(board);
    int best_score = maximizing ? -INT_MAX : INT_MAX;
    Move best_move;

    for (Move& move : possible_moves) {
        board.make_move(move, possible_moves);
        auto result = minimax(depth - 1, board, !maximizing);
        board.un_move(move);

        if (result.second > best_score && maximizing) {
            best_score = result.second;
            best_move = move;
        } else if (result.second < best_score && !maximizing) {
            best_score = result.second;
            best_move = move;
        }
    }

    return {best_move, best_score};
}














#endif //CHESS_MINIMAX_HPP
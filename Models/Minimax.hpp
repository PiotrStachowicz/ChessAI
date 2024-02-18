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

int count_material(Board &board, bool maximizing) {
    int white_material = 0;
    int black_material = 0;

    for (Piece &piece: board.pieces) {
        if (piece.alive) {
            switch (piece.color) {
                case true:  // White pieces
                    switch (piece.type) {
                        case 1:
                            white_material += pawn_value;
                            break;
                        case 2:
                            white_material += bishop_value;
                            break;
                        case 3:
                            white_material += knight_value;
                            break;
                        case 4:
                            white_material += rook_value;
                            break;
                        case 5:
                            white_material += queen_value;
                            break;
                    }
                    break;
                case false:  // Black pieces
                    switch (piece.type) {
                        case 1:
                            black_material += pawn_value;
                            break;
                        case 2:
                            black_material += bishop_value;
                            break;
                        case 3:
                            black_material += knight_value;
                            break;
                        case 4:
                            black_material += rook_value;
                            break;
                        case 5:
                            black_material += queen_value;
                            break;
                    }
                    break;
            }
        }
    }

    int white_advantage = white_material - black_material;

    if (maximizing && board.turn) return white_advantage;
    else if (!maximizing && board.turn) return -white_advantage;
    else if (maximizing && !board.turn) return -white_advantage;
    else return white_advantage;
}

int prio_attack(Move &move) {
    int value = 0;

    if(move.is_attack()) {
        value += 10 * (move.end_piece->type - move.start_piece->type) + 10;
    }


    return value;
}

int move_value(Move &move) {
    int result = 0;
    result += prio_attack(move);

    return result;
}

std::pair<Move, int> minimax(int depth, Board &board, bool maximizing, int alfa, int beta) {
    if (depth == 0) return {Move(), count_material(board, maximizing)};

    std::vector<Move> possible_moves = Move::generate_legal_moves(board);

    std::sort(possible_moves.begin(), possible_moves.end(),
              [](Move &move1, Move &move2) { return move_value(move1) < move_value(move2); });

    // Checkmate
    if (possible_moves.empty()) {
        return {Move(), maximizing ? -INT_MAX : INT_MAX};
    }

    int best_score = maximizing ? -INT_MAX : INT_MAX;
    Move best_move = possible_moves[0];

    for (Move &move: possible_moves) {
        board.make_move(move, possible_moves);
        auto result = minimax(depth - 1, board, !maximizing, alfa, beta);

        board.undo_move(move);

        if (maximizing) {
            if (result.second > best_score) {
                best_score = result.second;
                best_move = move;
            }

            if (best_score > alfa) {
                alfa = best_score;
            }
        } else {
            if (result.second < best_score) {
                best_score = result.second;
                best_move = move;
            }

            if (best_score < beta) {
                beta = best_score;
            }
        }

        if (beta <= alfa) break;
    }

    return {best_move, best_score};
}


#endif //CHESS_MINIMAX_HPP
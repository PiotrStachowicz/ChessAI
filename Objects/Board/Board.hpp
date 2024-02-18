// Piotr Stachowicz
#ifndef CHESS_BOARD_HPP
#define CHESS_BOARD_HPP
#pragma once
#include "../../Settings/Settings.hpp"
#include "../Move/Move.hpp"
#include "../Piece/Piece.hpp"

class Move;
class Piece;

template<typename T>
bool element_in(std::vector<T> &vec, T element) {
    return std::find(vec.begin(), vec.end(), element) != vec.end();
}

enum state { win, lose, nothing };

class Board {
public:
    std::vector<Piece> pieces;
    bool turn;
public:
    Board();

    explicit Board(const std::string &fen);

    void load_fen(const std::string &fen);

    Piece* piece_on_position(uint16_t position);

    bool make_move(Move& move, std::vector<Move>& legal);

    void undo_move(Move& move);

    state end();
};


#endif //CHESS_BOARD_HPP
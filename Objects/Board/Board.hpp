// Piotr Stachowicz
#ifndef CHESS_BOARD_HPP
#define CHESS_BOARD_HPP
#pragma once
#include "../../Settings/Settings.hpp"
#include "../Move/Move.hpp"
#include "../Piece/Piece.hpp"


class Board {
public:
    std::vector<Piece> pieces;
    bool turn;
public:
    Board();

    explicit Board(const std::string &fen);

    void load_fen(const std::string &fen);

    Piece* piece_on_position(uint16_t position);

    bool make_move(Move move);
    void un_move(Move move);

};


#endif //CHESS_BOARD_HPP
// Piotr Stachowicz
#ifndef CHESS_MOVE_HPP
#define CHESS_MOVE_HPP
#pragma once
#include "../../Settings/Settings.hpp"
#include "../Piece/Piece.hpp"

class Piece;

class Move {
public:
    uint16_t start;
    uint16_t end;
    Piece *start_piece;
    Piece *end_piece;
public:
    Move();

    Move(uint16_t start, uint16_t end, Piece *start_piece, Piece *end_piece);

    [[nodiscard]] bool left_castle(bool color) const;

    [[nodiscard]] bool right_castle(bool color) const;

    [[nodiscard]] bool skip(bool color) const;

    [[nodiscard]] bool takedown() const;

    bool operator==(const Move &move) const;
};

#endif //CHESS_MOVE_HPP
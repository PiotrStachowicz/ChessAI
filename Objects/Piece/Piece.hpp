// Piotr Stachowicz
#ifndef CHESS_PIECE_HPP
#define CHESS_PIECE_HPP
#pragma once
#include "../../Settings/Settings.hpp"
#include "../Move/Move.hpp"

class Move;
class Board;


class Piece {
public:
    static const uint8_t none;
    static const uint8_t pawn;
    static const uint8_t knight;
    static const uint8_t bishop;
    static const uint8_t rook;
    static const uint8_t queen;
    static const uint8_t king;
public:
    uint16_t position;
    uint8_t type;
    uint8_t promotion_type = Piece::none;
    bool color;
    bool alive;
    uint32_t move_count;
public:
    explicit Piece(uint16_t position);

    Piece(uint16_t position, uint8_t type, bool color, bool alive, bool move_count);

    [[nodiscard]] bool can_move(bool turn) const;

    void move(uint16_t new_position);

    void kill();

    void revive();

    [[nodiscard]] bool is_type(bool color, int type) const;

    bool operator==(const Piece &piece) const;
};


#endif //CHESS_PIECE_HPP
// Piotr Stachowicz
#ifndef CHESS_PIECE_HPP
#define CHESS_PIECE_HPP
#pragma once

#include "../../Settings/Settings.hpp"
#include "../Move/Move.hpp"


// Pre-define for linker
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
    uint32_t move_count;
    bool alive;
    bool color;
public:
    explicit Piece(uint16_t position);

    Piece(uint16_t position, uint8_t type, bool color, bool alive);

    [[nodiscard]] bool can_move(bool turn) const;

    void move(uint16_t new_position);

    void kill();

    void revive();

    [[nodiscard]] bool is_type(bool color, int type) const;

    bool operator==(const Piece &piece) const;
};


#endif //CHESS_PIECE_HPP
// Piotr Stachowicz
#ifndef CHESS_MOVE_HPP
#define CHESS_MOVE_HPP
#pragma once

#include "../../Settings/Settings.hpp"
#include "../Piece/Piece.hpp"
#include "../Board/Board.hpp"

// Pre-define for linker
class Board;
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

    [[nodiscard]] bool is_skip() const;

    [[nodiscard]] bool if_castle(bool color) const;

    [[nodiscard]] bool is_attack() const;

    static std::vector<Move> generate_legal_moves(Board &board);

    static std::vector<Move> generate_pseudo_legal_moves(Board &board);

    static std::vector<Move> slide_move(Board &board, Piece *start_piece, int start_direction, int end_direction, int max);

    static std::vector<Move> jump_move(Board &board, Piece *start_piece);

    static std::vector<Move> pawn_move(Board &board, Piece *start_piece);

    static std::vector<Move> castle_move(Board &board, Piece *start_piece);

    static bool piece_attacked(const std::vector<Move>& moves, Piece *piece);

    static int shannon_number(Board& board, int depth);

    bool operator==(const Move& move) const;
};

#endif //CHESS_MOVE_HPP
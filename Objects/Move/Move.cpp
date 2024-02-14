// Piotr Stachowicz
#include "Move.hpp"

Move::Move() : start(0), end(0), start_piece(nullptr), end_piece(nullptr) {}

Move::Move(uint16_t start, uint16_t end, Piece *start_piece, Piece *end_piece) :
        start(start), end(end), start_piece(start_piece), end_piece(end_piece) {}

bool Move::left_castle(bool color) const {
    if (!start_piece->is_type(color, Piece::king) || !end_piece->is_type(color, Piece::rook)) return false;
    if (start_piece->move_count != 0 || end_piece->move_count != 0) return false;
    return true;
}

bool Move::right_castle(bool color) const {
    if (!start_piece->is_type(color, Piece::king) || !end_piece->is_type(color, Piece::rook)) return false;
    if (start_piece->move_count != 0 || end_piece->move_count != 0) return false;
    return true;
}

bool Move::skip(bool color) const {
    if (end_piece->is_type(false, Piece::none) && start_piece->move_count == 1 && start_piece->color == color)
        return true;
    return false;
}

bool Move::takedown() const {
    if (end_piece->color != start_piece->color && !end_piece->is_type(false, Piece::none)) return true;
    return false;
}

bool Move::operator==(const Move &move) const {
    return move.end_piece == move.start_piece && move.start == start && move.end == end;
}


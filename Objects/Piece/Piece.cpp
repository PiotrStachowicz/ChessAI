// Piotr Stachowicz
#include "Piece.hpp"


const uint8_t Piece::none = 0;
const uint8_t Piece::pawn = 1;
const uint8_t Piece::knight = 2;
const uint8_t Piece::bishop = 3;
const uint8_t Piece::rook = 4;
const uint8_t Piece::queen = 5;
const uint8_t Piece::king = 6;


Piece::Piece(uint16_t position) : position(position), type(Piece::none), color(false), alive(false), move_count(0) {}

Piece::Piece(uint16_t position, uint8_t type, bool color, bool alive) : position(position), type(type), color(color),
                                                                        alive(alive), move_count(0) {}

bool Piece::can_move(bool turn) const { return type != Piece::none && turn == color && alive; }

void Piece::move(uint16_t new_position) { position = new_position; }

void Piece::kill() { alive = false; }

void Piece::revive() { alive = true; }

bool Piece::is_type(bool color1, int type1) const { return color == color1 && type == type1; }

bool Piece::operator==(const Piece &piece) const { return piece.color == color && piece.type == type; }

// Piotr Stachowicz
#include "Board.hpp"

Board::Board() : turn(true), pieces({}) {}

Board::Board(const std::string &fen) : turn(true), pieces({}) { load_fen(fen); }

void Board::load_fen(const std::string &fen) {
    uint16_t rank = 0, file = 0;

    std::map<char, int> dict = {
            {'p', Piece::pawn},
            {'n', Piece::knight},
            {'b', Piece::bishop},
            {'r', Piece::rook},
            {'k', Piece::king},
            {'q', Piece::queen}};

    for (const char &c: fen) {
        if (c == '/') {
            rank++;
            file = 0;
        } else {
            if (isdigit(c)) {
                file += c - '0';
            } else {
                int piece = dict[(char) tolower(c)];

                if (isupper(c)) pieces.emplace_back(rank * 8 + file, piece, true, true, 0);
                else pieces.emplace_back(rank * 8 + file, piece, false, true, 0);

                file++;
            }
        }
    }
    pieces.emplace_back(64);
}

Piece* Board::piece_on_position(uint16_t position) {
    for(Piece& piece : pieces) {
        if(piece.position == position && piece.alive) return &piece;
    }
    return &pieces[pieces.size()-1];
}

bool Board::make_move(Move move) {
    // Check if legal
    std::vector<Move> pseudo_legal = Move::generate_pseudo_legal_moves(*this);

    if(!element_in<Move>(pseudo_legal, move)) return false;

    // Castle Move Pieces
    if(move.if_castle(turn)) {
        int off1 = move.end == 63 || move.end == 7 ? 2 : -3;
        int off2 = move.end == 63 || move.end == 7 ? 1 : -2;

        move.start_piece->move(move.start + off1);
        move.end_piece->move(move.start + off2);

        move.end_piece->move_count++;
    }
    // Normal & En Passant Move Pieces
    else {
        move.start_piece->move(move.end);
        move.end_piece->kill();
    }

    move.start_piece->move_count++;

    // Change Turn
    turn = not turn;

    return true;
}

void Board::un_move(Move move) {
    move.start_piece->move_count--;
    move.end_piece->move_count--;

    move.start_piece->move(move.start);
    if(move.if_castle(move.start_piece->color)) {
        move.end_piece->move(move.end);
    }else {
        move.end_piece->move_count++;
        move.end_piece->revive();
    }

    turn = not turn;
}

// Piotr Stachowicz
#include "Move.hpp"

Move::Move() : start(0), end(0), start_piece(nullptr), end_piece(nullptr) {}

Move::Move(uint16_t start, uint16_t end, Piece *start_piece, Piece *end_piece) :
        start(start), end(end), start_piece(start_piece), end_piece(end_piece) {}

bool Move::if_castle(bool color) const {
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
    return move.end_piece == end_piece && move.start_piece == start_piece && move.start == start && move.end == end;
}

std::vector<Move> Move::generate_pseudo_legal_moves(Board &board) {
    std::vector<Move> result{};

    for (int start = 0; start < 64; start++) {
        Piece* start_piece = board.piece_on_position(start);

        if(start_piece->can_move(board.turn)) {
            std::vector<Move> piece_moves{};

            switch (start_piece->type) {
                case 5:
                    piece_moves = slide_move(board, start, start_piece, 0, 8, INT_MAX);
                    result.insert(result.end(), piece_moves.begin(), piece_moves.end());
                    break;
                case 4:
                    piece_moves = slide_move(board, start, start_piece, 0, 4, INT_MAX);
                    result.insert(result.end(), piece_moves.begin(), piece_moves.end());
                    break;
                case 3:
                    piece_moves = slide_move(board, start, start_piece, 4, 8, INT_MAX);
                    result.insert(result.end(), piece_moves.begin(), piece_moves.end());
                    break;
                case 6:
                    piece_moves = slide_move(board, start, start_piece, 0, 8, 1);
                    result.insert(result.end(), piece_moves.begin(), piece_moves.end());
                    piece_moves = castle_move(board, start, start_piece);
                    result.insert(result.end(), piece_moves.begin(), piece_moves.end());
                    break;
                case 2:
                    piece_moves = jump_move(board, start, start_piece);
                    result.insert(result.end(), piece_moves.begin(), piece_moves.end());
                    break;
                case 1:
                    switch (start_piece->color) {
                        case true:
                            piece_moves = pawn_move(board, start, start_piece);
                            result.insert(result.end(), piece_moves.begin(), piece_moves.end());
                            break;
                        case false:
                            piece_moves = pawn_move(board, start, start_piece);
                            result.insert(result.end(), piece_moves.begin(), piece_moves.end());
                            break;
                    }
                    break;
            }
        }
    }
    return result;
}

std::vector<Move> Move::slide_move(Board &board, int start, Piece* start_piece, int start_direction, int end_direction, int max) {
    std::vector<Move> result{};

    for(int i = start_direction; i < end_direction; i++) {
        bool takedown = false;

        for(int n = 0; n < std::min(distances[start][i], max); n++) {
            int end = start + directions[i] * (n + 1);

            Piece* endPiece = board.piece_on_position(end);

            if(endPiece->type == Piece::none && !takedown) result.emplace_back(start, end, start_piece, endPiece);
            else if(!endPiece->can_move(board.turn) && !takedown) {
                result.emplace_back(start, end, start_piece, endPiece);
                takedown = true;
            }
            else break;
        }
    }

    return result;
}

std::vector<Move> Move::jump_move(Board &board, int start, Piece* start_piece) {
    std::vector<Move> result{};

    std::vector<int> jumps = {start - 17, start - 15, start - 10, start - 6,
                         start + 17, start + 15, start + 10, start + 6};

    for(int jump : jumps) {
        Piece* end_piece = board.piece_on_position(jump);

        if (jump >= 0 && jump <= 63 && !end_piece->can_move(board.turn)) {
            result.emplace_back(start, jump, start_piece, end_piece);
        }
    }

    return result;
}

std::vector<Move> Move::pawn_move(Board &board, int start, Piece* start_piece) {
    std::vector<Move> result{};
    std::array<int, 3> pawn_movement_indexes{};

    if(board.turn) pawn_movement_indexes = {2, 4, 5};
    else pawn_movement_indexes = {3, 6, 7};

    int max = start_piece->move_count == 0 ? 2 : 1;

    for (int i = 0; i < 3; i++) {
        bool takedown = false;

        for (int n = 0; n < std::min(distances[start][pawn_movement_indexes[i]], max); n++) {
            int end = start + directions[pawn_movement_indexes[i]] * (n + 1);
            Piece* end_piece = board.piece_on_position(end);

            if ((abs(directions[pawn_movement_indexes[i]]) == 9 || abs(directions[pawn_movement_indexes[i]]) == 7) && n == 0) {
                if (!end_piece->can_move(board.turn) && !takedown && end_piece->type != Piece::none) {
                    result.emplace_back(start, end, start_piece, end_piece);
                    takedown = true;
                } else if (end_piece->type == Piece::none && !takedown) {
                    Piece* en_passant_piece = board.piece_on_position(start_piece->color ? end + 8 : end - 8);

                    if(en_passant_piece->alive && en_passant_piece->move_count == 1 && !en_passant_piece->can_move(board.turn)) {
                        result.emplace_back(start, end, start_piece, en_passant_piece);
                        takedown = true;
                    }
                } else break;
            } else {
                if (end_piece->type == Piece::none && !takedown) {
                    result.emplace_back(start, end, start_piece, end_piece);
                }
                else break;
            }
        }
    }

    return result;
}

std::vector<Move> Move::castle_move(Board &board, int start, Piece* start_piece) {
    std::vector<Move> result{};

    std::array<int, 6> indexes{};

    board.turn ? indexes = {56, 63, 57, 58, 61, 62} : indexes = {0, 7, 1, 2, 5, 6};

    Piece* rook_left = board.piece_on_position(indexes[0]);
    Piece* rook_right = board.piece_on_position(indexes[1]);
    Piece* piece1 = board.piece_on_position(indexes[2]);
    Piece* piece2 = board.piece_on_position(indexes[3]);
    Piece* piece3 = board.piece_on_position(indexes[4]);
    Piece* piece4 = board.piece_on_position(indexes[5]);

    if(start_piece->move_count == 0) {
        if(rook_left->can_move(board.turn) && rook_left->move_count == 0 && piece1->type == Piece::none && piece2->type == Piece::none) result.emplace_back(start, indexes[0], start_piece, rook_left);
        if(rook_right->can_move(board.turn) && rook_right->move_count == 0 && piece3->type == Piece::none && piece4->type == Piece::none) result.emplace_back(start, indexes[1], start_piece, rook_right);
    }

    return result;
}
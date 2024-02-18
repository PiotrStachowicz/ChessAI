// Piotr Stachowicz
#include "Move.hpp"
using namespace std;

Move::Move() : start(0), end(0), start_piece(nullptr), end_piece(nullptr) {}

Move::Move(uint16_t start, uint16_t end, Piece *start_piece, Piece *end_piece) : start(start), end(end), start_piece(start_piece), end_piece(end_piece) {}

bool Move::if_castle(bool color) const {
    if (!start_piece->is_type(color, Piece::king) || !end_piece->is_type(color, Piece::rook)) return false;
    if (start_piece->move_count != 0 || end_piece->move_count != 0) return false;
    return true;
}

bool Move::operator==(const Move &move) const { return move.end_piece == end_piece && move.start_piece == start_piece && move.start == start && move.end == end; }

vector<Move> Move::generate_pseudo_legal_moves(Board &board) {
    vector<Move> result{};

    for (Piece& piece : board.pieces) {

        if (piece.can_move(board.turn)) {
            vector<Move> piece_moves{};

            switch (piece.type) {
                case 5:
                    piece_moves = slide_move(board, &piece, 0, 8, INT_MAX);
                    result.insert(result.end(), piece_moves.begin(), piece_moves.end());
                    break;
                case 4:
                    piece_moves = slide_move(board, &piece, 0, 4, INT_MAX);
                    result.insert(result.end(), piece_moves.begin(), piece_moves.end());
                    break;
                case 3:
                    piece_moves = slide_move(board, &piece, 4, 8, INT_MAX);
                    result.insert(result.end(), piece_moves.begin(), piece_moves.end());
                    break;
                case 6:
                    piece_moves = slide_move(board, &piece, 0, 8, 1);
                    result.insert(result.end(), piece_moves.begin(), piece_moves.end());
                    piece_moves = castle_move(board, &piece);
                    result.insert(result.end(), piece_moves.begin(), piece_moves.end());
                    break;
                case 2:
                    piece_moves = jump_move(board, &piece);
                    result.insert(result.end(), piece_moves.begin(), piece_moves.end());
                    break;
                case 1:
                    piece_moves = pawn_move(board, &piece);
                    result.insert(result.end(), piece_moves.begin(), piece_moves.end());
                    break;
            }
        }
    }
    return result;
}

vector<Move> Move::slide_move(Board &board, Piece *start_piece, int start_direction, int end_direction, int max) {
    vector<Move> result{};
    int start = start_piece->position;

    for (int i = start_direction; i < end_direction; i++) {
        bool takedown = false;

        for (int n = 0; n < min(distances[start][i], max); n++) {
            int end = start + directions[i] * (n + 1);

            Piece *endPiece = board.piece_on_position(end);

            if (endPiece->type == Piece::none && !takedown) result.emplace_back(start, end, start_piece, endPiece);
            else if (!endPiece->can_move(board.turn) && !takedown) {
                result.emplace_back(start, end, start_piece, endPiece);
                takedown = true;
            } else break;
        }
    }

    return result;
}

vector<Move> Move::jump_move(Board &board, Piece *start_piece) {
    vector<Move> result{};
    int start = start_piece->position;

    vector<int> jumps = {start - 17, start - 15, start - 10, start - 6,
                              start + 17, start + 15, start + 10, start + 6};

    int start_file = start % 8;
    int start_rank = start / 8;

    for (int jump: jumps) {
        Piece *end_piece = board.piece_on_position(jump);

        int dest_file = jump % 8;
        int dest_rank = jump / 8;

        int file_diff = abs(dest_file - start_file);
        int rank_diff = abs(dest_rank - start_rank);

        if (((file_diff == 1 && rank_diff == 2) || (file_diff == 2 && rank_diff == 1)) && jump >= 0 && jump <= 63 &&
            (!end_piece->can_move(board.turn) || end_piece->type == Piece::none)) {
            result.emplace_back(start, jump, start_piece, end_piece);
        }
    }

    return result;
}

vector<Move> Move::pawn_move(Board &board, Piece *start_piece) {
    vector<Move> result{};
    array<int, 3> pawn_movement_indexes{};
    int start = start_piece->position;


    pawn_movement_indexes = board.turn ? array<int, 3>{2, 4, 5} : array<int, 3>{3, 6, 7};

    int max = start_piece->move_count == 0 ? 2 : 1;

    for (int i = 0; i < 3; i++) {
        bool takedown = false;

        for (int n = 0; n < min(distances[start][pawn_movement_indexes[i]], max); n++) {
            int direction = directions[pawn_movement_indexes[i]];
            int end = start + direction * (n + 1);
            Piece *end_piece = board.piece_on_position(end);

            if ((abs(direction) == 9 || abs(direction) == 7) && n == 0) {
                if (!end_piece->can_move(board.turn) && !takedown && end_piece->type != Piece::none) {
                    result.emplace_back(start, end, start_piece, end_piece);
                    takedown = true;
                } else if (end_piece->type == Piece::none && !takedown) {
                    Piece *en_passant_piece = board.piece_on_position(start_piece->color ? end + 8 : end - 8);

                    if (en_passant_piece->alive && en_passant_piece->type == Piece::pawn &&
                        en_passant_piece->move_count == 1 && !en_passant_piece->can_move(board.turn)) {
                        result.emplace_back(start, end, start_piece, en_passant_piece);
                        takedown = true;
                    }
                } else break;
            } else if (end_piece->type == Piece::none && !takedown && abs(direction) == 8) {
                result.emplace_back(start, end, start_piece, end_piece);
            } else break;
        }
    }

    return result;
}

vector<Move> Move::castle_move(Board &board, Piece *start_piece) {
    vector<Move> result{};
    int start = start_piece->position;

    array<int, 7> indexes{};

    board.turn ? indexes = {56, 63, 57, 58, 59, 61, 62} : indexes = {0, 7, 1, 2, 3, 5, 6};

    Piece *rook_left = board.piece_on_position(indexes[0]);
    Piece *rook_right = board.piece_on_position(indexes[1]);
    Piece *piece1 = board.piece_on_position(indexes[2]);
    Piece *piece2 = board.piece_on_position(indexes[3]);
    Piece *piece3 = board.piece_on_position(indexes[4]);
    Piece *piece4 = board.piece_on_position(indexes[5]);
    Piece *piece5 = board.piece_on_position(indexes[6]);


    if (start_piece->move_count == 0) {
        if (rook_left->can_move(board.turn) && rook_left->move_count == 0 && piece1->type == Piece::none &&
            piece2->type == Piece::none && piece3->type == Piece::none)
            result.emplace_back(start, indexes[0], start_piece, rook_left);

        if (rook_right->can_move(board.turn) && rook_right->move_count == 0 && piece4->type == Piece::none &&
            piece5->type == Piece::none)
            result.emplace_back(start, indexes[1], start_piece, rook_right);
    }

    return result;
}

vector<Move> Move::generate_legal_moves(Board &board) {
    vector<Move> pseudo_legal = generate_pseudo_legal_moves(board);
    board.turn = not board.turn;
    vector<Move> pseudo_legal_enemy = generate_pseudo_legal_moves(board);
    board.turn = not board.turn;
    vector<Move> legal{};

    Piece *king;
    for (Piece &piece: board.pieces) {
        if (piece.type == Piece::king && piece.color == board.turn) king = &piece;
    }

    bool checked = Move::piece_attacked(pseudo_legal_enemy, king);

    // Very slow...
    for (Move& move: pseudo_legal) {
        if (move.if_castle(board.turn) && !checked) {
            board.make_move(move, pseudo_legal);
            vector<Move> potential_attack = generate_pseudo_legal_moves(board);

            if (!piece_attacked(potential_attack, king) && !piece_attacked(potential_attack, move.end_piece)) legal.push_back(move);
            board.undo_move(move);
        } else {
            board.make_move(move, pseudo_legal);
            if (!piece_attacked(generate_pseudo_legal_moves(board), king)) legal.push_back(move);
            board.undo_move(move);
        }
    }

    return legal;
}

bool Move::piece_attacked(const vector<Move> &moves, Piece *piece) {
    for (const Move& move: moves) if (move.end_piece == piece) return true;
    return false;
}

bool Move::is_attack() const { return start_piece->color != end_piece->color && end_piece->type != Piece::none; }

int Move::shannon_number(Board &board, int depth) {
    if(depth == 0) return 0;
    int result = 0;

    vector<Move> moves = Move::generate_legal_moves(board);

    for(Move& move : moves) {
        result++;

        board.make_move(move, moves);
        result += shannon_number(board, depth - 1);
        board.undo_move(move);
    }

    return result;
}

bool Move::is_skip() const { return start_piece->type == Piece::pawn && abs(end - start) == 16; }

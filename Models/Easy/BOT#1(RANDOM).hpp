// Piotr Stachowicz
#include "../../Settings/Settings.hpp"
#include "../../Objects/Move/Move.hpp"
#include "../../Objects/Board/Board.hpp"
#include "../../Objects/Piece/Piece.hpp"


class RandomBot {
public:
    // Settings
    // (none)
public:

    static void generate_move(Board& board) {
        std::mt19937 mt(1);
        auto moves = Move::generate_legal_moves(board);
        std::cout << moves.size() << std::endl;
        int move_idx = mt() % moves.size();
        std::cout << move_idx << std::endl;
        board.make_move(moves[move_idx], moves);
    }
};


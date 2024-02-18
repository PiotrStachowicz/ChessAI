// Piotr Stachowicz
#include "Objects/Board/Board.hpp"
#include "Models/Easy/BOT#1(RANDOM).hpp"
#include "Models/Easy/BOT#2(MINIMAX).hpp"

using namespace std;

// Textures
sf::Texture wKingTexture, bKingTexture;
sf::Texture wQueenTexture, bQueenTexture;
sf::Texture wPawnTexture, bPawnTexture;
sf::Texture wKnightTexture, bKnightTexture;
sf::Texture wRookTexture, bRookTexture;
sf::Texture wBishopTexture, bBishopTexture;
map<pair<uint8_t, bool>, sf::Sprite> textures{};

// Sprites
sf::Sprite wKing, bKing;
sf::Sprite wQueen, bQueen;
sf::Sprite wPawn, bPawn;
sf::Sprite wKnight, bKnight;
sf::Sprite wRook, bRook;
sf::Sprite wBishop, bBishop;

void load_textures() {
    try {
        // Load Textures
        wKingTexture.loadFromFile("../Public/Pictures/white-king.png");
        bKingTexture.loadFromFile("../Public/Pictures/black-king.png");
        wQueenTexture.loadFromFile("../Public/Pictures/white-queen.png");
        bQueenTexture.loadFromFile("../Public/Pictures/black-queen.png");
        wPawnTexture.loadFromFile("../Public/Pictures/white-pawn.png");
        bPawnTexture.loadFromFile("../Public/Pictures/black-pawn.png");
        wKnightTexture.loadFromFile("../Public/Pictures/white-knight.png");
        bKnightTexture.loadFromFile("../Public/Pictures/black-knight.png");
        wRookTexture.loadFromFile("../Public/Pictures/white-rook.png");
        bRookTexture.loadFromFile("../Public/Pictures/black-rook.png");
        wBishopTexture.loadFromFile("../Public/Pictures/white-bishop.png");
        bBishopTexture.loadFromFile("../Public/Pictures/black-bishop.png");

        // Load Sprites
        wKing = sf::Sprite(wKingTexture);
        bKing = sf::Sprite(bKingTexture);
        wQueen = sf::Sprite(wQueenTexture);
        bQueen = sf::Sprite(bQueenTexture);
        wPawn = sf::Sprite(wPawnTexture);
        bPawn = sf::Sprite(bPawnTexture);
        wKnight = sf::Sprite(wKnightTexture);
        bKnight = sf::Sprite(bKnightTexture);
        wRook = sf::Sprite(wRookTexture);
        bRook = sf::Sprite(bRookTexture);
        wBishop = sf::Sprite(wBishopTexture);
        bBishop = sf::Sprite(bBishopTexture);

        // Scale
        float scale = 0.7;
        wKing.setScale(scale, scale);
        bKing.setScale(scale, scale);
        wQueen.setScale(scale, scale);
        bQueen.setScale(scale, scale);
        wPawn.setScale(scale, scale);
        bPawn.setScale(scale, scale);
        wKnight.setScale(scale, scale);
        bKnight.setScale(scale, scale);
        wRook.setScale(scale, scale);
        bRook.setScale(scale, scale);
        wBishop.setScale(scale, scale);
        bBishop.setScale(scale, scale);

        // Map textures
        textures = {
                {{Piece::pawn,   true},  wPawn},
                {{Piece::knight, true},  wKnight},
                {{Piece::bishop, true},  wBishop},
                {{Piece::rook,   true},  wRook},
                {{Piece::queen,  true},  wQueen},
                {{Piece::king,   true},  wKing},
                {{Piece::pawn,   false}, bPawn},
                {{Piece::knight, false}, bKnight},
                {{Piece::bishop, false}, bBishop},
                {{Piece::rook,   false}, bRook},
                {{Piece::queen,  false}, bQueen},
                {{Piece::king,   false}, bKing}
        };
    } catch (std::exception& e) {
        std::cerr << e.what();
    }
}

void game() {
    array<int, 64> option_tiles{0};

    Board board(START_FEN);

    // test
    //cout << Move::shannon_number(board, 5);

    std::deque<Move> history{};

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Chess Game");

    int size = WIDTH / 8;
    sf::RectangleShape rectangle(sf::Vector2f((float)size, (float)size));
    uint16_t start = 64;
    uintptr_t end;
    vector<Move> legal{};

    // Bot
    MinimaxBot sad_bot{};

    // Main Game Loop
    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    if (!history.empty()) {
                        board.undo_move(history.back());
                        history.pop_back();
                    }
                    break;

                case sf::Event::MouseButtonPressed:
                    if (start != 64) {
                        sf::Vector2<int> pos = sf::Mouse::getPosition(window);
                        end = pos.y / size * 8 + pos.x / size;

                        Piece *start_piece = board.piece_on_position(start);
                        Piece *end_piece = board.piece_on_position(end);
                        Piece *possible_passant1 = board.piece_on_position(start - 1);
                        Piece *possible_passant2 = board.piece_on_position(start + 1);

                        if (possible_passant1->type == Piece::pawn && !possible_passant1->can_move(board.turn) &&
                            possible_passant1->move_count == 1 &&
                            start_piece->type == Piece::pawn && end_piece->type == Piece::none &&
                            (board.turn && std::abs((int) end - start) == 9 ||
                             !board.turn && std::abs((int) end - start) == 7)) {
                            Move move(start, end, start_piece, possible_passant1);
                            if (board.make_move(move, legal)) {
                                history.push_back(move);
                                //history.push_back(sad_bot.generate_move(board));
                            }
                        } else if (possible_passant2->type == Piece::pawn && !possible_passant2->can_move(board.turn) &&
                                   possible_passant2->move_count == 1 &&
                                   start_piece->type == Piece::pawn && end_piece->type == Piece::none &&
                                   (board.turn && std::abs((int) end - start) == 7 ||
                                    !board.turn && std::abs((int) end - start) == 9)) {
                            Move move(start, end, start_piece, possible_passant2);

                            if (board.make_move(move, legal)) {
                                history.push_back(move);
                                //history.push_back(sad_bot.generate_move(board));
                            }
                        } else {
                            Move move(start, end, start_piece, end_piece);

                            if (board.make_move(move, legal)) {
                                history.push_back(move);
                                //history.push_back(sad_bot.generate_move(board));
                            }
                        }
                        for (int i = 0; i < 64; i++) option_tiles[i] = 0;
                        legal.clear();
                        start = 64;
                        if (board.end() != nothing) {
                            board = Board(START_FEN);
                            history.clear();
                        }
                    } else {
                        sf::Vector2<int> pos = sf::Mouse::getPosition(window);
                        start = pos.y / size * 8 + pos.x / size;
                        legal = Move::generate_legal_moves(board);
                        for (Move &move: legal) {
                            if (move.start == start) option_tiles[move.end] = 2;
                        }
                        option_tiles[start] = 1;
                    }
                    break;
            }
        }

        window.clear();

        // Draw Chess Board
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                rectangle.setPosition((float) j * (float) size, (float) i * (float) size);
                if (option_tiles[i * 8 + j] == 2) rectangle.setFillColor(Redish);
                else if (option_tiles[i * 8 + j] == 1) rectangle.setFillColor(Orangeish);
                else if ((i + j) % 2 == 0) rectangle.setFillColor(Green);
                else rectangle.setFillColor(Cream);

                window.draw(rectangle);
                Piece *piece = board.piece_on_position(i * 8 + j);

                textures[{piece->type, piece->color}].setPosition(j * (float) size + 0.15 * size, i * size + 0.15 * size);
                window.draw(textures[{piece->type, piece->color}]);
            }
        }

        window.display();
    }
}


int main() {
    compute_distances();
    load_textures();
    game();
    return 0;
}

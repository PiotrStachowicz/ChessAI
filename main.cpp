#include "Objects/Board/Board.hpp"
using namespace std;

// Textures
sf::Texture wKingTexture;
sf::Texture bKingTexture;
sf::Texture wQueenTexture;
sf::Texture bQueenTexture;
sf::Texture wPawnTexture;
sf::Texture bPawnTexture;
sf::Texture wKnightTexture;
sf::Texture bKnightTexture;
sf::Texture wRookTexture;
sf::Texture bRookTexture;
sf::Texture wBishopTexture;
sf::Texture bBishopTexture;

// Sprites
sf::Sprite wKing, bKing;
sf::Sprite wQueen, bQueen;
sf::Sprite wPawn, bPawn;
sf::Sprite wKnight, bKnight;
sf::Sprite wRook, bRook;
sf::Sprite wBishop, bBishop;

void load_textures() {
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
    wKing.setScale(0.5f, 0.5f);
    bKing.setScale(0.5f, 0.5f);
    wQueen.setScale(0.5f, 0.5f);
    bQueen.setScale(0.5f, 0.5f);
    wPawn.setScale(0.5f, 0.5f);
    bPawn.setScale(0.5f, 0.5f);
    wKnight.setScale(0.5f, 0.5f);
    bKnight.setScale(0.5f, 0.5f);
    wRook.setScale(0.5f, 0.5f);
    bRook.setScale(0.5f, 0.5f);
    wBishop.setScale(0.5f, 0.5f);
    bBishop.setScale(0.5f, 0.5f);
}

void game() {

    // Map textures
    map<pair<uint8_t, bool>, sf::Sprite> textures = {
            {{Piece::pawn, true}, wPawn},
            {{Piece::knight, true}, wKnight},
            {{Piece::bishop, true}, wBishop},
            {{Piece::rook, true}, wRook},
            {{Piece::queen, true}, wQueen},
            {{Piece::king, true}, wKing},
            {{Piece::pawn, false}, bPawn},
            {{Piece::knight, false}, bKnight},
            {{Piece::bishop, false}, bBishop},
            {{Piece::rook, false}, bRook},
            {{Piece::queen, false}, bQueen},
            {{Piece::king, false}, bKing}
    };

    Board board(Start_FEN);

    std::deque<Move> history{};

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Chess Game");

    int size = WIDTH / 8;
    sf::RectangleShape rectangle(sf::Vector2f((float)size, (float)size));
    uint16_t start = 64;
    uintptr_t end;

    // Main Game Loop
    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                if(!history.empty()) {
                    board.un_move(history.back());
                    history.pop_back();
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (start != 64) {
                    sf::Vector2<int> pos = sf::Mouse::getPosition(window);
                    end = pos.y / size * 8 + pos.x / size;

                    Piece* start_piece = board.piece_on_position(start);
                    Piece* end_piece = board.piece_on_position(end);
                    Move move(start, end, start_piece, end_piece);

                    if(board.make_move(move)) {
                        history.push_back(move);
                    }

                    start = 64;
                } else {
                    sf::Vector2<int> pos = sf::Mouse::getPosition(window);
                    start = pos.y / size * 8 + pos.x / size;
                }
            }
        }

        window.clear();

        // Draw Chess Board
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                rectangle.setPosition((float) j * (float)size, (float) i * (float)size);
                if ((i + j) % 2 == 0) rectangle.setFillColor(Green);
                else rectangle.setFillColor(Cream);

                window.draw(rectangle);
                Piece* piece = board.piece_on_position(i * 8 + j);

                textures[{piece->type, piece->color}].setPosition(j * (float)size + 0.2 * size, i * size + 0.2 * size);
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
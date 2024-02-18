// Piotr Stachowicz
#include "Settings.hpp"


// left | right | up | down | top-left | top-right | bottom-left | bottom-right
std::array<int, 8> directions = {-1, 1, -8, 8, -9, -7, 7, 9};

std::array<std::array<int, 8>, 8 * 8> distances{};

void compute_distances() {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int left = file;
            int right = 7 - file;
            int up = rank;
            int down = 7 - rank;

            int position = rank * 8 + file;

            distances[position][0] = left;
            distances[position][1] = right;
            distances[position][2] = up;
            distances[position][3] = down;
            distances[position][4] = std::min(left, up);
            distances[position][5] = std::min(right, up);
            distances[position][6] = std::min(left, down);
            distances[position][7] = std::min(right, down);
        }
    }
}

// Colors
const sf::Color Green(124, 174, 104);
const sf::Color Cream(240, 217, 181);
const sf::Color Redish(255, 99, 71);
const sf::Color Orangeish(255, 165, 0);

// Piotr Stachowicz
#ifndef CHESS_SETTINGS_HPP
#define CHESS_SETTINGS_HPP

#include <iostream>
#include <map>
#include <vector>
#include "SFML/Graphics.hpp"
#include <cmath>
#include <algorithm>
#include <array>
#include <deque>
#include <cstdint>
#include <random>

// Game Settings
#define WIDTH 1000
#define HEIGHT 1000
#define Start_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

extern std::array<int, 8> directions;
extern std::array<std::array<int, 8>, 8 * 8> distances;

extern void compute_distances();
extern const sf::Color Green;
extern const sf::Color Cream;
extern const sf::Color Redish;
extern const sf::Color Orangeish;

#endif //CHESS_SETTINGS_HPP
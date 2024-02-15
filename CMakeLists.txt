cmake_minimum_required(VERSION 3.26)
project(Chess)

# Standard
set(CMAKE_CXX_STANDARD 23)

# Executables
add_executable(Chess
        main.cpp
        Objects/Piece/Piece.hpp
        Objects/Piece/Piece.cpp
        Objects/Board/Board.hpp
        Objects/Board/Board.cpp
        Objects/Move/Move.hpp
        Objects/Move/Move.cpp
        Settings/Settings.hpp
        Settings/Settings.cpp
        "Models/Easy/BOT#1(RANDOM).hpp"
        "Models/Easy/BOT#2(MINIMAX).hpp"
        Models/Minimax.hpp
)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(C:/SFML/include/SFML)
target_link_libraries(Chess sfml-system sfml-window sfml-graphics sfml-audio)

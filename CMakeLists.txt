cmake_minimum_required(VERSION 3.23)
project (canasta_cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(canasta_cpp main.cpp card.cpp deck.cpp player.cpp game.cpp meld.cpp ui_enums.h)

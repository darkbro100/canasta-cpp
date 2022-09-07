cmake_minimum_required(VERSION 3.23)
project (canasta_cpp)

set(CMAKE_CXX_STANDARD 11)

add_executable(canasta_cpp main.cpp card.cpp deck.cpp)

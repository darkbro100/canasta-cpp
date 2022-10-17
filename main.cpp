//****************************************************
//* Name:  Paul Guarnieri                         *
//* Project:  Canasta #1    *
//* Class:  CMPS 366 01 - Organization of Programming Languages               *
//* Date:  10-17-2022                   *
//****************************************************

#include <iostream>
#include "game.h"
#include "game_serializer.h"

using namespace Canasta;

static char code;

static void handleInput() {
    std::cin >> code;
    code = (char) (std::tolower(code));
}

int main() {
    Game g;

    printf("Hit [y] if you would like to load in a game from a file. Any other input will create a new game.\n");
    handleInput();
    if (code == 'y') {
        printf("Please enter the name of the file.\n");
        std::string fileName;
        std::getline(std::cin >> std::ws, fileName);

        g.getStockPile()->clear();
        loadGame(fileName.c_str(), g);
    } else
        g.setup();

    g.start();
    while (g.isStarted())
        g.startTurn();

    std::cout << "Exiting..." << std::endl;
    return 0;
}
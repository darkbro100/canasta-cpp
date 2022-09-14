#include <iostream>
#include "game.h"
#include "game_serializer.h"
#include <fstream>

using namespace Canasta;

/**
 * TODO:
 * - Add help menu
 * - Add game saving (ties into serialization)
 *
 */

/**
 * TODO: not sure if this will stay the same. dont know if i want to move the keyboard input into its own class/file or keep it how the way it is and pass the char code to the game object and have the game object handle the specific input there
 */
static char code;

static void handleInput() {
    std::cin >> code;
    code = (char) (std::tolower(code));
}

int main() {
    Game g;

    printf("Hit [y] if you would like to load in a game from a file\n");
    handleInput();
    if (code == 'y') {
        printf("Please enter the name of the file.\n");
        std::string fileName;
        std::getline(std::cin >> std::ws, fileName);

        g.getStockPile()->clear();
        readFile(fileName.c_str(), g);
    }
    g.start();
    while (g.isStarted())
        g.startTurn();

    std::cout << "Exiting..." << std::endl;
    return 0;
}
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
    g.setup();

    printf("Game setup!\n\nHit [y] to start, anything else to quit.\n");

    handleInput();
    if (code == 'y') {
        g.start();
        while (g.isStarted())
            g.startTurn();
    } else {
        std::cout << "Exiting..." << std::endl;
        exit(0);
    }
}
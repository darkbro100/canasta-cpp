#include <iostream>
#include "game.h"

using namespace Canasta;

/**
 * TODO:
 * - Add AI turns, right now the CPU just does nothing every turn.
 * - Add "Going out"
 * - Actually increment round counter, start keeping score of points
 * - Add in serialization of games
 * - Add in the remaining few edge cases
 *      - If the last card drawn from the stockpile is a red 3
 *      - Stockpile is empty and both players are unable to use the discard pile.
 *          - In this situation specifically, a player must use the discard pile to ensure the game continues
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
        while (g.isStarted()) {
            g.startTurn();
        }
    } else {
        std::cout << "Exiting..." << std::endl;
        exit(0);
    }
}
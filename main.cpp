#include <iostream>
#include "game.h"

using namespace Canasta;

/**
 * TODO: not sure if this will stay the same. dont know if i want to move the keyboard input into its own class/file or keep it how the way it is and pass the char code to the game object and have the game object handle the specific input there
 */
static char code;

static void handleInput() {
    std::cin >> code;
    code = (char) (std::tolower(code));
}

//int main() {
//    Game g;
//    g.setup();
//
//    Player *p = g.getPlayer(0);
//    Meld *m = p->createMeld(1);
//    m->addCard(Card(Suit::CLUBS, 1));
//    m->addCard(Card(Suit::DIAMONDS, 1));
//    m->addCard(Card(Suit::DIAMONDS, 2));
//    m->addCard(Card(static_cast<Suit>(-1), -1));
//    m->addCard(Card(static_cast<Suit>(-1), -1));
//    m->addCard(Card(static_cast<Suit>(-1), -1));
//
//    std::cout << "wildcards: " << m->getWildCards() << std::endl;
//    std::cout << *p->getMeld(1) << std::endl;
//}

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
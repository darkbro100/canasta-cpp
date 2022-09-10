#include <iostream>
#include "game.h"

using namespace Canasta;

int main() {
    Game g;

    g.setup();

    std::cout << "Player1:" << std::endl;
    std::cout << *g.getPlayer(0)->getHand() << std::endl;

    std::cout << "Player2:" << std::endl;
    std::cout << *g.getPlayer(1)->getHand() << std::endl;

    std::cout << "Discard pile:" << std::endl;
    std::cout << *g.getDiscardPile() << std::endl;

    std::cout << "Stockpiles pile:" << std::endl;
    std::cout << *g.getStockPile() << std::endl;
}

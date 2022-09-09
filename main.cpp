#include <iostream>
#include "deck.h"

int main() {

    // create deck, shuffle, then draw every card
    Canasta::Deck d = Canasta::Deck();
    d.shuffle();

    while(!d.empty()) {
        std::shared_ptr<Canasta::Card> c = d.drawCard();
        std::cout << "drew: " << *c << std::endl;
    }

}

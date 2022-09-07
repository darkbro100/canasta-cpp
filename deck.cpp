#include "deck.h"
#include <iostream>

namespace Canasta {
Deck::Deck() {
    int size = this->cards.size();
    std::cout << "constructor called " << size << std::endl;
}
};

#include "deck.h"
#include <iostream>

namespace Canasta {
    Deck::Deck() {
        size_t size = this->cards.size();
        std::cout << "constructor called " << size << std::endl;
    }
};

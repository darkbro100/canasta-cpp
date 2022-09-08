#include <iostream>
#include <cstdlib>
#include "card.h"
#include "deck.h"
#include <ctime>
#include <memory>

int main() {
    srand(time(nullptr));
    
    Canasta::Deck d = Canasta::Deck();

    for(int i = 0; i < 52; i++) {
        int rank = (i % 13) + 1;

        Canasta::Suit s = static_cast<Canasta::Suit>(rand() % Canasta::Suit::DIAMONDS);
        Canasta::Card c = Canasta::Card(s, rank);

        std::cout << "ADded card " << c << std::endl;
        d.addCard(c);
    }


    while(!d.empty()) {
        std::shared_ptr<Canasta::Card> ptr = d.drawCard();
        std::cout << "drawing card... " << std::endl;
        std::cout << *ptr << std::endl;
    }

}

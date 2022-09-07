#include <iostream>
#include <cstdlib>
#include "card.h"
#include "deck.h"
#include <time.h>

int main() {
    srand(time(NULL));
    
    Canasta::Deck d = Canasta::Deck();
    
    for(int i = 0; i < 52; i++) {
        int rank = (i % 13) + 1;
        Canasta::Suit s = static_cast<Canasta::Suit>(rand() % Canasta::Suit::DIAMONDS);
        Canasta::Card c = Canasta::Card(s, rank);
        
        std::cout << c << std::endl;
    }
}

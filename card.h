#pragma once

#include <string>
#include <iostream>

namespace Canasta {

static char FACES[3] = {'J', 'Q', 'K'};

enum Suit {
    CLUBS,
    HEARTS,
    SPADES,
    DIAMONDS
};

class Card {
private:
    const Suit s;
    const int rank;

public:
    Card(Suit s, int rank): s(s), rank(rank) {}
    Suit getSuit();
    int getRank();
};

// easily print to stdout
std::ostream& operator<<(std::ostream& os, Suit s);
std::ostream& operator<<(std::ostream& os, Card & c);

};

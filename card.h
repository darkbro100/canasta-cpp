#pragma once

#include <string>
#include <iostream>

namespace Canasta {

    static char FACES[3] = {'K', 'Q', 'J'};

    enum Suit {
        CLUBS,
        HEARTS,
        SPADES,
        DIAMONDS
    };

    class Card {
    private:
        Suit s;
        int rank;

    public:
        Card(Suit s, int rank);
        Card(Canasta::Card &c);
        Card(Canasta::Card && c) noexcept ;

        Suit getSuit();

        int getRank();

        bool isFace();

        bool isAce();

        bool isJoker();

        Card &operator=(Card &&c) noexcept ;
    };

    // easily print to stdout
    std::ostream &operator<<(std::ostream &os, Suit s);
    std::ostream &operator<<(std::ostream &os, Card &c);

};

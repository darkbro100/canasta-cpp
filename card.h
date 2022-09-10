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

        /**
         * @return Return rank, or value of the card
         */
        int getRank();

        /**
         * <h1>Wild cards:</h1> <b>Deuces (2) and Jokers</b> - these can be used as any rank except as 3.
         */
        bool isWildcard();

        /**
         * <h1>Natural cards:</h1> <b>A, K, Q, J, 10, 9, 8, 7, 6, 5, 4.</b>
         */
        bool isNatural();

        /**
         * <h1>Red 3s (Heart, Diamond)</h1> They can never be part of any meld. They cannot be discarded.
         */
        bool isRedThree();

        /**
         * <h1>Black 3s (Spades, Clubs)</h1> They can be part of a meld containing only black 3s (and no wildcards). They can also be used to freeze the discard pile.
         */
        bool isBlackThree();

        Card &operator=(Card &&c) noexcept ;
    };

    // easily print to stdout
    std::ostream &operator<<(std::ostream &os, Suit s);
    std::ostream &operator<<(std::ostream &os, Card &c);

};

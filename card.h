#pragma once

#include <string>
#include <iostream>
#include <map>

#define RED_THREE_POINTS 100
#define BLACK_THREE_POINTS 3

namespace Canasta {

    static char FACES[3] = {'K', 'Q', 'J'};

    static const std::map<int, int> POINT_MAP = {
            {-1, 50},
            {1, 20},
            {2, 20},
            {4, 5},
            {5, 5},
            {6, 5},
            {7, 5},
            {8, 10},
            {9, 10},
            {10, 10},
            {11, 10},
            {12, 10},
            {13, 10}
    };

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

        Card(Canasta::Card &&c) noexcept;

        Suit getSuit();

        /**
         *
         * @return Point value of the card
         */
        int getPoints();

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

        /**
         * @return True if this card can freeze the discard pile (black 3 & wildcard)
         */
        bool canFreezeDiscard();

        Card &operator=(Card &&c) noexcept;

        bool operator<(const Card &c) const;

        bool operator>(const Card &c) const;
    };

    // easily print to stdout
    std::ostream &operator<<(std::ostream &os, Suit s);

    std::ostream &operator<<(std::ostream &os, Card &c);

};

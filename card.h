#pragma once

#include <string>
#include <iostream>
#include <map>
#include "ui_enums.h"

#define RED_THREE_POINTS 100
#define BLACK_THREE_POINTS 3

namespace Canasta {

    /**
     * An array of characters representing the face cards. Specific to this translation unit
     */
    static char FACES[3] = {'K', 'Q', 'J'};

    /**
     * A static map containing the point values of each card. Specific to this translation unit
     */
    static const std::map<int, int> POINT_MAP = {
            {-1, 50},
            {1,  20},
            {2,  20},
            {4,  5},
            {5,  5},
            {6,  5},
            {7,  5},
            {8,  10},
            {9,  10},
            {10, 10},
            {11, 10},
            {12, 10},
            {13, 10}
    };

    class Card {
    public:
        /**
         * Default constructor. Card MUST be constructed using a Suit and a Rank.
         * @param s Suit
         * @param rank Rank
         */
        Card(Suit s, int rank);

        /**
         * Copy constructor
         * @param c Card to copy from
         */
        Card(Canasta::Card &c);

        /**
         * Move constructor
         * @param c Rvalue reference to Card to move from
         */
        Card(Canasta::Card &&c) noexcept;

        /**
         * @return Suit value of the card
         */
        Suit getSuit();

        /**
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

        /**
         * Move assignment operator
         * @param c Rvalue reference to move from
         * @return Reference to this card instance
         */
        Card &operator=(Card &&c) noexcept;

        /**
         * Less than operator for card. Used specifically for vector sorting
         * @param c Card to compare to
         * @return True if less than, false if greataer than (not entirely true, look at src implementation to see)
         */
        bool operator<(const Card &c) const;

        /**
         * Greater than operator for card. Used specifically for vector sorting
         * @param c Card to compare to
         * @return True if greater than, false if less than (not entirely true, look at src implementation to see)
         */
        bool operator>(const Card &c) const;

    private:
        Suit s;
        int rank;
    };

    /**
     * Easily be able to print to an ostream
     * @param os ostream to print to
     * @param s Suit to print
     * @return Ostream
     */
    std::ostream &operator<<(std::ostream &os, Suit s);

    /**
     * Easily be able to print to an ostream
     * @param os ostream to print to
     * @param c Card to print
     * @return Ostream
     */
    std::ostream &operator<<(std::ostream &os, Card &c);

};

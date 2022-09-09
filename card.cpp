#include <iostream>
#include "card.h"

namespace Canasta {

    bool Card::isAce() {
        return this->rank == 1;
    }

    bool Card::isFace() {
        return this->rank > 10;
    }

    bool Card::isJoker() {
        return this->rank == -1;
    }

    int Card::getRank() {
        return this->rank;
    }

    Suit Card::getSuit() {
        return this->s;
    }

    Card &Card::operator=(Card &&c) noexcept {
        this->rank = c.rank;
        this->s = c.s;

        // no idea if this is necessary
        c.rank = 0;
        c.s = static_cast<Suit>(0);

        return *this;
    }

    Card::Card(Card &&c) noexcept {
        rank = c.rank;
        s = c.s;

        c.rank = 0;
        c.s = static_cast<Suit>(0);
    }

    Card::Card(Suit s, int rank) {
        this->rank = rank;
        this->s = s;
    }

    Card::Card(Card &c) {
        this->rank = c.rank;
        this->s = c.s;
    }

    std::ostream &operator<<(std::ostream &os, Card &c) {

        // handle joker here
        if (c.getRank() == -1) {
            os << "Joker";
            return os;
        }

        // append rank
        if (c.getRank() == 1) {
            os << "A";
        } else if (c.getRank() > 10) {
            int index = 13 - c.getRank();
            char c = FACES[index];
            os << c;
        } else {
            os << c.getRank();
        }

        // append suit
        os << c.getSuit();

        return os;
    }

    std::ostream &operator<<(std::ostream &os, Suit s) {
        switch (s) {
            case CLUBS   :
                os << "C";
                break;
            case HEARTS:
                os << "H";
                break;
            case SPADES :
                os << "S";
                break;
            case DIAMONDS  :
                os << "D";
                break;
            default    :
                os.setstate(std::ios_base::failbit);
        }
        return os;
    }

};

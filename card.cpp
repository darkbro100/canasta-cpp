#include <iostream>
#include "card.h"

namespace Canasta {

    int Card::getRank() {
        return this->rank;
    }

    Suit Card::getSuit() {
        return this->s;
    }

    std::ostream &operator<<(std::ostream &os, Card &c) {

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

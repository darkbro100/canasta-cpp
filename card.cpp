#include <iostream>
#include "card.h"

namespace Canasta {

    int Card::getPoints() {
        if(isRedThree())
            return RED_THREE_POINTS;
        if(isBlackThree())
            return BLACK_THREE_POINTS;

        return POINT_MAP.at(rank);
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

    bool Card::canFreezeDiscard() {
        return isWildcard() || isBlackThree();
    }

    bool Card::isWildcard() {
        return rank == -1 || rank == 2;
    }

    bool Card::isNatural() {
        return !isRedThree() && !isBlackThree() && !isWildcard();
    }

    bool Card::isRedThree() {
        return rank == 3 && (s == Suit::DIAMONDS || s == Suit::HEARTS);
    }

    bool Card::isBlackThree() {
        return rank == 3 && (s == Suit::CLUBS || s == Suit::SPADES);
    }

    bool Card::operator<(const Card & c) const {
        if(rank == 2 || rank == -1)
            return true;
        if(c.rank == 2 || c.rank == -1)
            return false;

        return this->rank < c.rank;
    }

    bool Card::operator>(const Card & c) const {
        if(rank == 2 || rank == -1)
            return false;
        if(c.rank == 2 || c.rank == -1)
            return true;

        return this->rank > c.rank;
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

//
// Created by pauli on 9/11/2022.
//

#ifndef CANASTA_CPP_MELD_H
#define CANASTA_CPP_MELD_H

#define CANASTA_COUNT 7
#define MELD_COUNT 3

#include "deck.h"

namespace Canasta {
    class Meld : public Deck {
    private:
        int rank;
    public:
        Meld(int rank) : Deck(false), rank(rank) {}

        void addCard(Card card) override;

        int getRank();

        bool isNaturalCanasta();
        bool isCanasta();
    };

    class RedThreeMeld : public Meld {
    public:
        RedThreeMeld() : Meld(3) {}
        void addCard(Card card) override;
    };

    class BlackThreeMeld : public Meld {
    public:
        BlackThreeMeld() : Meld(3) {}
        void addCard(Card card) override;
    };
}

#endif //CANASTA_CPP_MELD_H

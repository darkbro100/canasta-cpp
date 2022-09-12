//
// Created by pauli on 9/11/2022.
//

#ifndef CANASTA_CPP_MELD_H
#define CANASTA_CPP_MELD_H

#define CANASTA_COUNT 7

#include "deck.h"

namespace Canasta {
    class Meld : public Deck {
    private:
        int rank;
    public:
        Meld(int rank) : Deck(false), rank(rank) {}
        void addCard(Card card) override;
        int getRank();
        bool isCanasta();
    };
}

#endif //CANASTA_CPP_MELD_H

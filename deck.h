#pragma once

#include <vector>
#include "card.h"
#include <memory>

#define DECK_SIZE 54

namespace Canasta {
    class Deck {
    private:
        std::vector<Card> cards;
    public:
        void shuffle();
        void addCard(Card &card);

        std::shared_ptr<Card> drawCard();

        bool empty();

        Deck();

        friend std::ostream &operator<<(std::ostream &os, Deck &d);
    };
};

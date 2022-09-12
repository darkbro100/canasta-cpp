#pragma once

#include <vector>
#include "card.h"
#include <memory>

#define DECK_SIZE 108

namespace Canasta {
    class Deck {
    private:
        std::vector<Card> cards;
    public:
        void shuffle();

        virtual void addCard(Card card);

        std::shared_ptr<Card> drawCard();
        std::shared_ptr<Card> topCard();
        int getWildCards();

        void removeCard(std::vector<Card>::iterator it);
        void clear();
        std::vector<Card>::iterator begin();
        std::vector<Card>::iterator end();
        bool empty();
        size_t count();

        Deck() : Deck(true) {}
        Deck(bool init);

        Card & operator[](int);
        friend std::ostream &operator<<(std::ostream &os, Deck &d);
    };
};

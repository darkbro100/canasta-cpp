#include "deck.h"
#include <memory>
#include <algorithm>
#include <random>

namespace Canasta {

    void Deck::addCard(Card & card) {
        cards.emplace_back(card.getSuit(), card.getRank());
    }

    /**
     * Draws the top card from the deck and removes it.
     *
     * TODO: will probably end up switching to just storing the cards as pointers tbh
     *
     * @returns Shared pointer to the card, otherwise nullptr if the deck is empty.
     */
    std::shared_ptr<Card> Deck::drawCard() {
        if (cards.empty())
            return nullptr;

        Card & c = cards.at(cards.size() - 1);
        std::shared_ptr<Card> ptr = std::make_shared<Card>(c);
        cards.erase(cards.end());
        return ptr;
    }

    bool Deck::empty() {
        return cards.empty();
    }

    Deck::Deck() {
        this->cards.reserve(DECK_SIZE);

        // clubs is first entry in enum
        int suit = Suit::CLUBS;

        // add all 52 playing cards
        for (int i = 0; i < 52; i++) {
            int rank = (i % 13) + 1;
            if(rank == 1 && i > 0)
                suit++;

            cards.emplace_back(static_cast<Suit>(suit), rank);
        }

        // add jokers
        for(int i = 0; i < 2; i++)
            cards.emplace_back(static_cast<Suit>(-1), -1);
    }

    std::ostream & operator<<(std::ostream &os, Deck &d) {
        for(int i = DECK_SIZE-1; i >= 0; i--) {
            Card & c = d.cards[i];
            os << c;

            if(i-1 >= 0)
                os << ", ";
        }

        return os;
    }

    void Deck::shuffle() {
        std::shuffle(cards.begin(), cards.end(), std::mt19937(std::random_device()()));
    }
};

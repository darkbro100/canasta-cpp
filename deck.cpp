#include "deck.h"
#include <memory>
#include <algorithm>
#include <random>

namespace Canasta {

    void Deck::addCard(Card card) {
        cards.emplace(cards.begin(), card.getSuit(), card.getRank());
//        cards.emplace_back(card.getSuit(), card.getRank());
    }

    std::shared_ptr<Card> Deck::drawCard() {
        if (cards.empty())
            return nullptr;

        auto it = --cards.end();
        Card c(*it);
        std::shared_ptr<Card> ptr = std::make_shared<Card>(c);
        cards.erase(it);

        return ptr;
    }

    bool Deck::empty() {
        return cards.empty();
    }

    Deck::Deck(bool init) {
        this->cards.reserve(
                DECK_SIZE); //we always want to reserve DECK_SIZE amount of cards for our vector (avoid copy constructor)

        if (init) {
            // clubs is first entry in enum
            int suit = Suit::CLUBS;

            // canasta uses 2 full decks of cards
            for (int i = 0; i < 2; i++) {
                // add all 52 playing cards
                for (int j = 0; j < 52; j++) {
                    int rank = (j % 13) + 1;
                    if (rank == 1 && j > 0)
                        suit++;

                    if (suit > Suit::DIAMONDS)
                        suit = Suit::CLUBS;

                    cards.emplace_back(static_cast<Suit>(suit), rank);
                }

                // add jokers
                for (int j = 0; j < 2; j++)
                    cards.emplace_back(static_cast<Suit>(-1), -1);
            }
        }
    }

    std::ostream &operator<<(std::ostream &os, Deck &d) {
        for (auto it = d.cards.begin(); it != d.cards.end(); it++) {
            os << *it;

            if (it != d.cards.end() - 1) {
                os << " ";
            }
        }

        return os;
    }

    void Deck::shuffle() {
        std::shuffle(cards.begin(), cards.end(), std::mt19937(std::random_device()()));
    }

    size_t Deck::count() {
        return cards.size();
    }

    std::vector<Card>::iterator Deck::begin() {
        return cards.begin();
    }

    std::vector<Card>::iterator Deck::end() {
        return cards.end();
    }

    void Deck::clear() {
        cards.clear();
    }

    std::shared_ptr<Card> Deck::topCard() {
        if (cards.empty())
            return nullptr;

        auto it = --cards.end();
        Card c(*it);
        std::shared_ptr<Card> ptr = std::make_shared<Card>(c);

        return ptr;
    }

    int Deck::getWildCards() {
        int c = 0;
        for(auto & it : *this) {
            if(it.isWildcard())
                c++;
        }

        return c;
    }

    void Deck::removeCard(std::vector<Card>::iterator it) {
        cards.erase(it);
    }

    Card &Deck::operator[](int i) {
        return cards.at(i);
    }

};

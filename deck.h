#pragma once

#include <vector>
#include "card.h"
#include <memory>

#define DECK_SIZE 108

namespace Canasta {
    class Deck {
    public:

        /**
         * Randomizes the order of the deck
         */
        void shuffle();

        /**
         * Adds a card to the deck
         * @param card Card to add. Passed by value
         */
        virtual void addCard(Card card);

        /**
         * Draws the top card from the deck and removes it.
         * @returns Shared pointer to the card, otherwise nullptr if the deck is empty.
         */
        std::shared_ptr<Card> drawCard();

        /**
        * Same exact functionality as #drawCard, does not remove card from deck though
        * @return Top card
        */
        std::shared_ptr<Card> topCard();

        /**
         * @return The number of wildcards that reside in this Deck
         */
        int getWildCards();

        /**
         * Removes a card from the deck, given an iterator
         * @param it Iterator pointing to the card in the deck
         */
        void removeCard(std::vector<Card>::iterator it);

        /**
         * Clears the deck and removes every card
         */
        void clear();

        /**
         * @return An iterator pointing to the beginning of the deck
         */
        std::vector<Card>::iterator begin();

        /**
         * @return An iterator pointing to the end of the deck
         */
        std::vector<Card>::iterator end();

        /**
         * @return True if the deck is empty, false otherwise
         */
        bool empty();

        /**
         * @return Number of cards inside the deck
         */
        size_t count();

        /**
         * Default constructor, will just init the deck with all the cards needed
         */
        Deck() : Deck(true) {}

        /**
        * Constructing a Deck
        * @param init If init is true, the deck will be populated with all the cards needed to play Canasta. Otherwise, it will be an empty deck.
        */
        explicit Deck(bool init);

        /**
         * Operator to reference a card at a specific index
         * @return Reference to the card at that index
         */
        Card &operator[](int);

        /**
         * An easy way to print a Deck out to an ostream
         * @param os Reference to the ostream
         * @param d Deck to print
         * @return The OStream used
         */
        friend std::ostream &operator<<(std::ostream &os, Deck &d);

    private:
        std::vector<Card> cards;
    };
};

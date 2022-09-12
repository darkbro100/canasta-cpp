//
// Created by pauli on 9/9/2022.
//

#ifndef CANASTA_CPP_PLAYER_H
#define CANASTA_CPP_PLAYER_H

#include "meld.h"

namespace Canasta {
    class Player {
    private:
        std::vector<Meld*> melds;
        Deck * hand;
        int points;
    public:
        Player();
        ~Player();
        Player(Player & p);
        Player(Player && p) noexcept;

        int getPoints();

        void setPoints(int points);

        /**
         * Draws a card from the given deck.
         * @param deck
         */
        void drawCard(Deck * deck);

        /**
         * Get a meld that a player has made
         * @param rank Rank that the meld was created with
         * @return Pointer to the meld, returns nullptr if it DNE
         */
        Meld * getMeld(int rank);

        /**
         * Creates a meld given a rank. Returns an already existing Meld if it's rank matches the one provided
         * @param rank Rank of the meld
         * @return Created meld, or existing meld with same rank
         */
        Meld * createMeld(int rank);

        /**
         * Checks if we can create a meld, given a Card from the discard pile
         * @param card Card from pile
         * @return True if we can create the meld
         */
        bool canCreateMeld(Card card);

        /**
         * Checks if we can create a Meld using the cards in our hand
         * @return Rank of meld, if it exists, -1 otherwise
         */
        int canCreateMeld();

        /**
         * @return Cards in the player's hand
         */
        Deck * getHand();
    };
}

#endif //CANASTA_CPP_PLAYER_H

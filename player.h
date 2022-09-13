//
// Created by pauli on 9/9/2022.
//

#ifndef CANASTA_CPP_PLAYER_H
#define CANASTA_CPP_PLAYER_H

#include "meld.h"

namespace Canasta {
    class Player {
    private:
        std::vector<Meld *> melds;
        Deck *hand;
        int points;
    public:
        Player();

        ~Player();

        Player(Player &p);

        Player(Player &&p) noexcept;

        int getPoints();

        void setPoints(int points);

        /**
         * Draws from the deck, continues to draw if you get a red three
         * @param deck Deck to draw from
         * @param takeAll If true, takes all the cards from the deck as opposed to only 1 (or more if red 3)
         */
        bool drawCard(Deck *deck, bool takeAll = false);

        Meld * getRedThreeMeld();
        Meld * getBlackThreeMeld();

        /**
         * Get a meld that a player has made
         * @param rank Rank that the meld was created with
         * @return Pointer to the meld, returns nullptr if it DNE
         */
        Meld *getMeld(int rank);

        /**
         * Creates a meld given a rank. Returns an already existing Meld if it's rank matches the one provided
         * @param rank Rank of the meld
         * @param redThree Create specific red 3 meld
         * @param blackThree Create specific black 3 meld
         *
         * @return Created meld, or existing meld with same rank
         */
        Meld *createMeld(int rank, bool redThree = false, bool blackThree = false);

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
         * Checks if there is a meld that is created that we can add to
         * @return The rank of a meld this player can add to, -1 if none exist
         */
        int checkMeld();

        /**
         * @return Cards in the player's hand
         */
        Deck *getHand();

        /**
         * Helper function to return the melds
         */
        std::vector<Meld*> & getMelds();

        /**
         * @return True if the player can go out
         */
        bool canGoOut();
    };
}

#endif //CANASTA_CPP_PLAYER_H

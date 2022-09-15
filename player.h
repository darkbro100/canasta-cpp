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
        bool isOut;
    public:
        Player();

        ~Player();

        Player(Player &p);

        Player(Player &&p) noexcept;

        /**
         * @return Points the player has earned
         */
        int getPoints() const;

        bool isPlayerOut() const;

        void setIsOut(bool out);

        /**
         * @return Calculates the points a player would earn
         */
        int calculatePoints();

        /**
         * Sets the player's points
         * @param points  Points to set to
         */
        void setPoints(int points);

        /**
         * Draws from the deck, continues to draw if you get a red three
         * @param deck Deck to draw from
         * @param takeAll If true, takes all the cards from the deck as opposed to only 1 (or more if red 3)
         */
        bool drawCard(Deck *deck, bool takeAll = false);

        /**
         * @return The meld specifically for red three's
         */
        Meld *getRedThreeMeld();

        /**
         * @return The meld specifically for black three's
         */
        Meld *getBlackThreeMeld();

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
        std::vector<Meld *> &getMelds();

        /**
         * @return True if the player can go out
         */
        bool canGoOut();

        /**
        * Checks which of the player's melds they can add to. Will be added to the supplied vector. Used for the AI calculating what melds to manipulate
        * @param ret Reference to vector of card's ranks.
        */
        void canAddToMelds(std::vector<int> &ret);
    };
}

#endif //CANASTA_CPP_PLAYER_H

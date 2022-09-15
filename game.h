//
// Created by pauli on 9/9/2022.
//

#ifndef CANASTA_CPP_GAME_H
#define CANASTA_CPP_GAME_H

#include "deck.h"
#include "player.h"
#include "ui_enums.h"

#define MAX_PLAYERS 2
#define HUMAN_PLAYER 0
#define CPU_PLAYER 1

namespace Canasta {
    class Game {
    public:
        /**
         * Default constructor for a Game
         */
        Game();
        /**
         * Destructor for a Game
         */
        ~Game();

        /**
         * Chooses which player that will move first during a new round.
         */
        void choosePlayer();

        /**
         * @return The current turn (or round) that we are on
         */
        int getCurrentTurn();

        /**
         * Sets the current turn (or round) to what is given
         *
         * @param turn Turn to set to
         */
        void setTurn(int turn);

        /**
         * @return A pointer to the stock pile
         */
        Deck *getStockPile();

        /**
         * @return A pointer to the discard pile
         */
        Deck *getDiscardPile();

        /**
         * @return The current player to play, given their index
         */
        int getCurrentPlayerIndex();

        /**
         * @param index Set the current player to play, given their index
         */
        void setCurrentPlayerIndex(int index);

        /**
        * Get a player playing the game
        * @param player Index in the array (0, 1)
        * @return Player object associated with the index
        */
        Player *getPlayer(int player);

        /**
         * @return A pointer to the current player who is playing
         */
        Player *getCurrentPlayer();

        /**
         * Sets up the game to a state where it can be played
         */
        void setup();

        /**
         * @return True if the game should stop, false otherwise.
         */
        bool shouldStop();

        /**
         * @return True if the game has been started, false otherwise
         */
        bool isStarted();

        /**
         * Starts the game
         */
        void start();

        /**
         * Ends the game
         */
        void endGame();

        // Turn related function calls

        /**
         * Starts a given turn for a given round
         */
        void startTurn();

        /**
         * AI turn calculations are done here
         */
        void aiTurn();

        /**
         * Called when the player (human) needs to draw a card
         * @param p Pointer to that player
         */
        void drawTurn(Player *p);

        /**
         * Called when the player (human) needs to do melds
         * @param p Pointer to that player
         */
        void meldTurn(Player *p);

        /**
         * Called when the player (human) needs to discard a card
         * @param p Pointer to that player
         */
        void discardTurn(Player *p);
    private:
        bool started;
        int currentPlayer;
        int startingPlayer;
        int currentTurn;
        Deck *stockPile, *discardPile;
        Player **players;
    };
}

#endif //CANASTA_CPP_GAME_H
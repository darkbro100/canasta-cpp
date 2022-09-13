//
// Created by pauli on 9/9/2022.
//

#ifndef CANASTA_CPP_GAME_H
#define CANASTA_CPP_GAME_H

#include "deck.h"
#include "player.h"
#include "ui_enums.h"

#define MAX_PLAYERS 2

namespace Canasta {
    class Game {
    private:
        bool started;
        int currentPlayer;
        int currentTurn;
        Deck * stockPile, * discardPile;
        Player ** players;
    public:
        Game();
        ~Game();

        void choosePlayer();
        int getCurrentTurn();

        Deck * getStockPile();
        Deck * getDiscardPile();
        Player * getCurrentPlayer();

        bool shouldStop();
        bool isStarted();

        void start();
        void endGame();

        // Turn related function calls
        void startTurn();
        void aiTurn();
        void drawTurn(Player * p);
        void meldTurn(Player * p);
        void discardTurn(Player * p);

        /**
         * Get a player playing the game
         * @param player Index in the array (0, 1)
         * @return Player object associated with the index
         */
        Player * getPlayer(int player);

        void setup();

        static void canAddToMelds(Player* player, std::vector<int> & ret);
    };
}

#endif //CANASTA_CPP_GAME_H
//
// Created by pauli on 9/9/2022.
//

#ifndef CANASTA_CPP_GAME_H
#define CANASTA_CPP_GAME_H

#include "deck.h"
#include "player.h"

#define MAX_PLAYERS 2

namespace Canasta {
    class Game {
    private:
        Deck * stockPile, * discardPile;
        Player ** players;
    public:
        Game();
        ~Game();

        Deck * getStockPile();

        Deck * getDiscardPile();

        /**
         * Get a player playing the game
         * @param player Index in the array (0, 1)
         * @return Player object associated with the index
         */
        Player * getPlayer(int player);
    };
}

#endif //CANASTA_CPP_GAME_H
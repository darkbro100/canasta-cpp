//
// Created by pauli on 9/9/2022.
//

#include "game.h"

namespace Canasta {
    Player *Game::getPlayer(int player) {
        return players[player];
    }

    Game::Game() {
        stockPile = new Deck(false);
        discardPile = new Deck(false);

        players = new Player *[MAX_PLAYERS];
        for (int i = 0; i < MAX_PLAYERS; i++) {
            players[i] = new Player();
        }
    }

    Game::~Game() {
        delete stockPile;
        delete discardPile;
        delete[] players;
    }

    Deck *Game::getDiscardPile() {
        return discardPile;
    }

    Deck *Game::getStockPile() {
        return stockPile;
    }
}
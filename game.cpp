//
// Created by pauli on 9/9/2022.
//

#include "game.h"
#include <random>

static int randomPlayer() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 1);

    return dist(mt);
}

namespace Canasta {
    Player *Game::getPlayer(int player) {
        return players[player];
    }

    Game::Game() {
        stockPile = new Deck();
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

    void Game::setup() {
        currentTurn = 1;

        for(int i =0; i < MAX_PLAYERS; i++)
            players[i]->setPoints(0);

        for(int i = 0; i < 3; i++)
            stockPile->shuffle();

        // draw 15 cards for each player
        for(int i = 0; i < 15; i++) {
            std::shared_ptr<Card> card1 = stockPile->drawCard();
            std::shared_ptr<Card> card2 = stockPile->drawCard();

            players[0]->getHand()->addCard(*card1);
            players[1]->getHand()->addCard(*card2);
        }

        std::shared_ptr<Card> c = stockPile->drawCard();
        discardPile->addCard(*c);

        while(c->isRedThree() || c->isWildcard()) {
            c = stockPile->drawCard();
            discardPile->addCard(*c);
        }

        choosePlayer();
    }

    int Game::getCurrentTurn() {
        return currentTurn;
    }

    void Game::choosePlayer() {
        if(currentTurn != 1) {
            int p1 = players[0]->getPoints();
            int p2 = players[1]->getPoints();
            currentPlayer = (p1 > p2 ? 0 : (p2 > p1 ? 1 : randomPlayer()));
        } else {
            currentPlayer = randomPlayer();
        }

        std::cout << "chosen player: " << currentPlayer << std::endl;
    }
}
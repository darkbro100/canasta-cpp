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

static const char * messages[4] = {
        "[1] Save the game\n",
        "[2] Take turn\n",
        "[3] Ask for help (only before human player plays)\n",
        "[4] Quit the game\n"
};
/**
 * Displays all pre round information specified at
 * @see https://pages.ramapo.edu/~amruth/teaching/opl/projects/canasta/problem.html
 * @return The code entered after pre round input was gathered
 */
static int displayPre() {
    for(auto & message : messages)
        printf("%s", message);

    int code;
    std::cin >> code;

    // if we fail to find something, set code to -1
    if(std::cin.fail()) {
        code = -1;
        std::cin.clear();
        std::cin.ignore();
    }

    return code;
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
        std::cout << "Setting game up..." << std::endl;
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
    }

    Player * Game::getCurrentPlayer() {
        return players[currentPlayer];
    }

    void Game::start() {
        started = true;
    }

    void Game::startTurn() {
        Player * p = this->getCurrentPlayer();
        PreCommands code;
        code = static_cast<PreCommands>(displayPre() - 1);

        switch(code) {
            case SAVE:
                std::cout << "Saving the game..." << std::endl;
                return;
            case HELP:
                std::cout << "Displaying help menu..." << std::endl;
                return;
            case QUIT:
                std::cout<< "Quitting" << std::endl;
                started = false;
                return;
            case TAKE:
                break;
            default:
                std::cout << "Unknown command..." << std::endl;
                return;
        }

        std::cout << "Your Hand: " << *players[0]->getHand() << std::endl;
        std::cout << "CPU Hand: " << *players[1]->getHand() << std::endl;
        std::cout << "Stockpile (top): " << *stockPile->topCard() << std::endl;
        std::cout << "Discard (top): " << *discardPile->topCard() << std::endl;

        //CPU players turn
        if(currentPlayer == 1) {
            //calculate what the cpu should do here
            std::cout << "Calculating best move..." << std::endl;
        } else {
            // Human Players turn

            //
            std::shared_ptr<Card> discardTop = discardPile->topCard();
            Meld * canAddToMeld = p->getMeld(discardTop->getRank());
            bool canUseDiscard = p->canCreateMeld(*discardTop);
            int meld = p->canCreateMeld();

            // if the player cannot use the discard pile or cannot add to a current meld using the discard pile, then auto draw a card for the player
            if(!canUseDiscard && !canAddToMeld) {

            }

            std::cout << "can meld from discard: " << canUseDiscard << std::endl;
            std::cout << "normal melds: " << meld << std::endl;

            if(!discardTop->isNatural() || p->getMeld(discardTop->getRank()) == nullptr) {
                std::cout << "i1" << std::endl;
            } else {
                std::cout << "i2" << std::endl;
                p->canCreateMeld(*discardTop);
            }

            std::cout << "Draw card, Do meld stuff, Discard card" << std::endl;
            int i;
            std::cin >> i;
        }

        // change to next player
        currentPlayer++;
        if(currentPlayer >= MAX_PLAYERS)
            currentPlayer = 0;
    }

    bool Game::isStarted() {
        return started;
    }

    /**
     * This assumes that the player is the human player
     */
    void Game::drawTurn() {
        Player * p = getCurrentPlayer();
        std::shared_ptr<Card> discardTop = discardPile->topCard();
        Meld * canAddToMeld = p->getMeld(discardTop->getRank());
        bool canUseDiscard = p->canCreateMeld(*discardTop);
    }

    void Game::meldTurn() {

    }

    void Game::discardTurn() {

    }

    void Game::aiTurn() {

    }
}
//
// Created by pauli on 9/9/2022.
//

#include "game.h"
#include "input_handler.h"
#include <random>
#include <algorithm>

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
        currentTurn = 0;
        started = false;
        currentPlayer = 0;
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

        for (int i = 0; i < MAX_PLAYERS; i++)
            players[i]->setPoints(0);

        for (int i = 0; i < 3; i++)
            stockPile->shuffle();

        // create "melds" for red 3s
        players[0]->createMeld(-1, true);
        players[1]->createMeld(-1, true);

        // draw 15 cards for each player
        for (int i = 0; i < 15; i++) {
            players[0]->drawCard(stockPile);
            players[1]->drawCard(stockPile);
        }

        // after players have received their cards, add red 3s to the correct "meld"
        for (auto it = players[0]->getHand()->begin(); it != players[0]->getHand()->end(); it++) {
            if (it->isRedThree()) {
                players[0]->getRedThreeMeld()->addCard(*it);
                players[0]->getHand()->removeCard(it);
            }
        }

        for (auto it = players[1]->getHand()->begin(); it != players[1]->getHand()->end(); it++) {
            if (it->isRedThree()) {
                players[1]->getRedThreeMeld()->addCard(*it);
                players[1]->getHand()->removeCard(it);
            }
        }

        // sort both players hands (for looks lol)
        std::sort(players[0]->getHand()->begin(), players[0]->getHand()->end(), std::greater<Card>());
        std::sort(players[1]->getHand()->begin(), players[1]->getHand()->end(), std::greater<Card>());

        std::shared_ptr<Card> c = stockPile->drawCard();
        discardPile->addCard(*c);

        while (c->isRedThree() || c->isWildcard()) {
            c = stockPile->drawCard();
            discardPile->addCard(*c);
        }

        choosePlayer();
    }

    int Game::getCurrentTurn() {
        return currentTurn;
    }

    void Game::choosePlayer() {
        if (currentTurn != 1) {
            int p1 = players[0]->getPoints();
            int p2 = players[1]->getPoints();
            currentPlayer = (p1 > p2 ? 0 : (p2 > p1 ? 1 : randomPlayer()));
        } else {
            currentPlayer = randomPlayer();
        }
    }

    Player *Game::getCurrentPlayer() {
        return players[currentPlayer];
    }

    void Game::start() {
        started = true;
    }

    void Game::startTurn() {
        Player *p = this->getCurrentPlayer();
        PreCommands code = displayPreRound();

        switch (code) {
            case SAVE:
                std::cout << "Saving the game..." << std::endl;
                return;
            case HELP:
                std::cout << "Displaying help menu..." << std::endl;
                return;
            case QUIT:
                std::cout << "Quitting" << std::endl;
                started = false;
                return;
            case TAKE:
                break;
            default:
                std::cout << "Unknown command..." << std::endl;
                return;
        }

        std::cout << "Your Hand: " << *players[0]->getHand() << std::endl;
        std::cout << "CPU Hand: " << *players[1]->getHand() << std::endl << std::endl;

        std::cout << "Your Melds: ";
        for (auto &it: players[0]->getMelds()) {
            if (it->empty()) // the melds for both 3s will show up by default
                continue;
            std::cout << *it << " ";
        }
        std::cout << std::endl;

        std::cout << "CPU Melds: ";
        for (auto &it: players[1]->getMelds()) {
            if (it->empty()) // the melds for both 3s will show up by default
                continue;
            std::cout << *it << " ";
        }
        std::cout << std::endl << std::endl;

        std::cout << "Stockpile (top): " << *stockPile->topCard() << std::endl;
        std::cout << "Discard (top): " << *discardPile->topCard() << std::endl
                  << std::endl; // additional endl for a line space

        //CPU players turn
        if (currentPlayer == 1) {
            //calculate what the cpu should do here
            std::cout << "Calculating best move..." << std::endl;
        } else {
            // Human Players turn

            // First do the drawing part of the player's turn
            drawTurn(p);
            std::cout << std::endl;

            // Afterwards, ask the player to do anything with melds
            meldTurn(p);
            std::cout << std::endl;

            // finally, make player discard card
            discardTurn(p);
            std::cout << std::endl;
        }

        // change to next player
        currentPlayer++;
        if (currentPlayer >= MAX_PLAYERS)
            currentPlayer = 0;
    }

    bool Game::isStarted() {
        return started;
    }

    /**
     * This assumes that the player is the human player
     */
    void Game::drawTurn(Player *p) {
        std::shared_ptr<Card> discardTop = discardPile->topCard();
        Meld *canAddToMeld = p->getMeld(discardTop->getRank());
        bool canUseDiscard = p->canCreateMeld(*discardTop);

        if (discardTop->isNatural() && (canAddToMeld || canUseDiscard)) {
            DrawCommands command = displayDrawOptions();

            if (command == DrawCommands::STOCK) {
                goto draw;
            } else if (command == DrawCommands::DISCARD) {

                if (canAddToMeld) {
                    canAddToMeld->addCard(*discardTop);
                    std::cout << "You added the top card from the discard pile (" << *discardTop
                              << ") to an existing meld." << std::endl;
                } else {
                    Meld *m = p->createMeld(discardTop->getRank());
                    m->addCard(*discardTop);

                    auto it = p->getHand()->begin();
                    while (it != p->getHand()->end()) {
                        size_t temp = m->count();
                        m->addCard(*it);

                        // if the card was successfully added, remove from hand
                        if (temp != m->count())
                            p->getHand()->removeCard(it);
                        else
                            it++;

                        // only start the meld
                        if (m->count() >= 3)
                            break;
                    }

                    std::cout << "You created a meld using the top card from the discard pile (" << *discardTop << ")."
                              << std::endl;
                }

                // remove card from discard pile (added to meld), then add remaining cards from discard pile to hand
                discardPile->drawCard();
                p->drawCard(discardPile, true);

                return;
            } else {
                std::cout << "Unknown command..." << std::endl;
                drawTurn(p);
            }
        }

        draw:
        std::vector<Card>::iterator oldEnd = p->getHand()->end();
        p->drawCard(stockPile, false);
        std::vector<Card>::iterator newend = p->getHand()->end();
        std::cout << "You drew: ";
        for (auto it = oldEnd; it != newend; it++) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }

    void Game::meldTurn(Player *p) {
        int meld = p->canCreateMeld();
        int addToMeld = p->checkMeld();

        // determine what the player wants to do with melds. if they choose to do nothing or can't do anything, stop here
        MeldCommands cmd = displayMeldOptions(meld != -1, addToMeld != -1);
        if (cmd == MeldCommands::NONE)
            return;

        switch (cmd) {
            case ADD: {
                int selected = selectMeld(p->getMelds(), "Select a meld to add to.");
                Meld *m = p->getMelds()[selected];

                std::cout << "Selected meld: " << *m << std::endl;
                selected = selectCard(p->getHand(), "Select a card to add to that meld.");

                size_t temp = m->count();
                Card &c = (*p->getHand())[selected];
                m->addCard(c);

                if (temp == m->count()) {
                    std::cout << "Could not add card " << c << " to meld." << std::endl;
                    meldTurn(p);
                    break;
                }

                std::cout << "Successfully added card " << c << " to meld." << std::endl;
                int i = 0;
                for (auto it = p->getHand()->begin(); it != p->getHand()->end(); it++) {
                    if (i == selected) {
                        p->getHand()->removeCard(it);
                        break;
                    }
                    i++;
                }

                break;
            }
            case CREATE: {
                int selected = selectCard(p->getHand(), "Select a card to create a meld with.");
                Card &c = (*p->getHand())[selected];
                Meld m = c.isBlackThree() ? BlackThreeMeld() : Meld(c.getRank());

                for (auto &it: *p->getHand())
                    m.addCard(it);

                if(m.count() < 3) {
                    std::cout << "You cannot create a meld. The most you can do is " << m << std::endl;
                    meldTurn(p);
                    break;
                }

                Meld * realMeld = p->createMeld(c.getRank(), false, c.isBlackThree());
                auto it = p->getHand()->begin();
                while(it != p->getHand()->end()) {
                    size_t tmp = realMeld->count();
                    realMeld->addCard(*it);

                    if(tmp != realMeld->count())
                        p->getHand()->removeCard(it);
                    else
                        it++;
                }

                std::cout << "Created new meld: " << *realMeld << std::endl;
                break;
            }
            default: {
                std::cout << "Unknown command..." << std::endl;
                meldTurn(p);
            }
        }
    }

    void Game::discardTurn(Player *p) {
        Deck *deck = p->getHand();

        int selected = selectCard(deck, "Select a card to move to the discard pile.");
        Card &c = (*deck)[selected];
        std::cout << "You discarded card: " << c << std::endl;

        int i = 0;
        for (auto it = deck->begin(); it != deck->end(); it++) {
            if (i == selected) {
                deck->removeCard(it);
                break;
            }
            i++;
        }

        discardPile->addCard(c);
    }

    void Game::aiTurn() {

    }
}
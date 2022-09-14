//
// Created by pauli on 9/9/2022.
//

#include "game.h"
#include "input_handler.h"
#include <random>
#include <algorithm>
#include <sstream>

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
        for (int i = 0; i < MAX_PLAYERS; i++) {
            Player *p = players[i];

            auto it = p->getHand()->begin();
            while (it != p->getHand()->end()) {
                if (it->isRedThree()) {
                    p->getRedThreeMeld()->addCard(*it);
                    p->getHand()->removeCard(it);
                } else {
                    it++;
                }
            }
        }

        // sort both players hands (for looks lol)
        for (int i = 0; i < MAX_PLAYERS; i++)
            std::sort(players[i]->getHand()->begin(), players[i]->getHand()->end(), std::greater<Card>());

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
        if (currentTurn == 1) {
            currentPlayer = randomPlayer();
            startingPlayer = currentPlayer;
            return;
        }

        int p1 = players[0]->getPoints();
        int p2 = players[1]->getPoints();

        currentPlayer = (p1 > p2 ? 0 : (p2 > p1 ? 1 : randomPlayer()));
        startingPlayer = currentPlayer;
    }

    Player *Game::getCurrentPlayer() {
        return players[currentPlayer];
    }

    void Game::start() {
        started = true;
    }

    void Game::startTurn() {
        if (shouldStop()) {
            std::cout
                    << "The game is ending because it is in a state where both players can no longer interact with the stock/discard piles."
                    << std::endl;
            endGame();
            return;
        }

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

        std::cout << "Current Round: " << currentTurn << std::endl;
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

        std::cout << "Stockpile (top): " << *stockPile->topCard() << " + " << stockPile->count() - 1 << " more"
                  << std::endl;
        std::cout << "Discard (top): " << *discardPile->topCard() << " + " << discardPile->count() - 1 << " more"
                  << std::endl
                  << std::endl; // additional endl for a line space

        //CPU players turn
        if (currentPlayer == 1) {

            //calculate what the cpu should do here
            aiTurn();

        } else { // Human player

            //Check if player can go out and ask them if they want to
            if (p->canGoOut()) {
                int shouldQuit = displayShouldGoOut();
                if (shouldQuit == 1) {
                    p->setIsOut(true);
                    endGame();
                    return;
                }
            }

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

        if (currentPlayer == startingPlayer) {
            currentTurn++;
            std::cout << "==== ROUND OVER ====" << std::endl;
            for (int i = 0; i < MAX_PLAYERS; i++) {
                p = players[i];

                int points = p->getPoints();
                int earned = p->calculatePoints();
                p->setPoints(earned + points);

                const char *str = (i == 0 ? "Player" : "AI");
                std::cout << str << " earned " << earned << " points this round." << std::endl;
                std::cout << str << " earned " << p->getPoints() << " points in total." << std::endl;
                std::cout << std::endl;
            }
            std::cout << std::endl;
            choosePlayer();
        }
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

        if (!discardTop->canFreezeDiscard() && (canAddToMeld || canUseDiscard)) {
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
                        if (m->count() >= MELD_COUNT)
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
        bool shouldEnd = p->drawCard(stockPile, false);
        if (shouldEnd) {
            endGame();
            return;
        }
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

                if (m.count() < MELD_COUNT) {
                    std::cout << "You cannot create a meld. The most you can do is " << m << std::endl;
                    meldTurn(p);
                    break;
                }

                int meldCount = createMeldWith(m.count());

                Meld *realMeld = p->createMeld(c.getRank(), false, c.isBlackThree());
                auto it = p->getHand()->begin();
                while (it != p->getHand()->end()) {
                    size_t tmp = realMeld->count();
                    realMeld->addCard(*it);

                    if (tmp != realMeld->count())
                        p->getHand()->removeCard(it);
                    else
                        it++;

                    if (realMeld->count() >= meldCount)
                        break;
                }

                std::cout << "Created new meld: " << *realMeld << std::endl;
                break;
            }
            default: {
                std::cout << "Unknown command..." << std::endl;
                break;
            }
        }
        meldTurn(p);
    }

    void Game::discardTurn(Player *p) {
        Deck *deck = p->getHand();

        if (deck->empty()) {
            std::cout << "You do not have any cards to discard." << std::endl;
        } else {
            int selected = selectCard(deck, "Select a card to move to the discard pile.");
            Card &c = (*deck)[selected];
            discardPile->addCard(c);
            std::cout << "You discarded card: " << c << std::endl;

            int i = 0;
            for (auto it = deck->begin(); it != deck->end(); it++) {
                if (i == selected) {
                    deck->removeCard(it);
                    break;
                }
                i++;
            }
        }
    }

    /**
     * Called when the AI needs to take a turn. AI will progress in these ways:
     * <ul>
     * <li>Draw card from either stockpile or discard pile
     * <li>Create/add to melds
     * <li>Discard card
     * </ul>
     */
    void Game::aiTurn() {
        std::stringstream stream;
        Player *ai = getCurrentPlayer();

        // only run the decision-making process if the AI can't go out
        if (!ai->canGoOut()) {
            bool tookDiscard = false;

            std::shared_ptr<Card> discardTop = discardPile->topCard();
            Meld *canAddToMeld = ai->getMeld(discardTop->getRank());
            bool canUseDiscard = ai->canCreateMeld(*discardTop);

            /**
             * Deciding what to do with drawing
             *
             * We don't do calculations for stockpile
             * because it is technically "face down"
             */
            // if the discard pile is not frozen, and we can do something with it
            if (!discardTop->canFreezeDiscard() && (canAddToMeld || canUseDiscard)) {

                // usually if you can interact with the discard pile in any kind of way it is beneficial
                if (canAddToMeld) {
                    canAddToMeld->addCard(*discardTop);
                    stream << "AI used discard pile to add to a meld: " << *canAddToMeld << std::endl;
                } else {
                    Meld *newMeld = ai->createMeld(discardTop->getRank());
                    newMeld->addCard(*discardTop);

                    auto it = ai->getHand()->begin();
                    while (it != ai->getHand()->end()) {
                        size_t tmp = newMeld->count();
                        newMeld->addCard(*it);

                        if (tmp != newMeld->count()) {
                            ai->getHand()->removeCard(it);
                        } else {
                            it++;
                        }

                        if (newMeld->count() >= MELD_COUNT)
                            break;
                    }

                    stream << "AI used discard pile to create a new meld: " << *newMeld << std::endl;
                }

                tookDiscard = true;
                discardPile->drawCard();
                ai->drawCard(discardPile, true);
            } else {
                std::vector<Card>::iterator oldEnd = ai->getHand()->end();
                bool shouldEnd = ai->drawCard(stockPile);
                if (shouldEnd) {
                    endGame();
                    return;
                }
                std::vector<Card>::iterator newend = ai->getHand()->end();
                std::cout << "The AI could not use the discard pile and drew: ";
                for (auto it = oldEnd; it != newend; it++) {
                    std::cout << *it << " ";
                }
                std::cout << std::endl;
            }

            /**
             *
             * Deciding what to do with melds
             *
             */
            int canMake = ai->canCreateMeld();
            while (canMake != -1) {
                Meld *newMeld = ai->createMeld(canMake, false, canMake == 3);

                auto it = ai->getHand()->begin();
                while (it != ai->getHand()->end()) {
                    size_t tmp = newMeld->count();
                    newMeld->addCard(*it);

                    if (tmp != newMeld->count()) {
                        ai->getHand()->removeCard(it);
                    } else {
                        it++;
                    }

                    if (newMeld->count() >= MELD_COUNT)
                        break;
                }
                stream << "The AI created a new meld: " << *newMeld << std::endl;
                canMake = ai->canCreateMeld();
            }

            std::vector<int> melds;
            canAddToMelds(ai, melds);

            if (!melds.empty()) {
                for (int i: melds) {
                    Meld *meld = i == 3 ? ai->getBlackThreeMeld() : ai->getMeld(i);
                    if (!meld)
                        continue;

                    size_t startingCount = meld->count();
                    auto it = ai->getHand()->begin();
                    while (it != ai->getHand()->end()) {
                        size_t tmp = meld->count();
                        meld->addCard(*it);

                        if (tmp != meld->count()) {
                            ai->getHand()->removeCard(it);
                        } else {
                            it++;
                        }
                    }

                    if (startingCount != meld->count())
                        stream << "The AI added to meld: " << *meld << std::endl;
                }
            }

            /**
             *
             * Deciding what card to discard
             *
             */
            if (ai->getHand()->empty()) {
                stream << "The AI has no cards to add to the discard pile." << std::endl;
            } else {
                Player *human = getPlayer(0); // Get human player here
                std::vector<Card>::iterator lowest = ai->getHand()->end();
                int lowestPoint = 1000;

                for (auto it = ai->getHand()->begin(); it != ai->getHand()->end(); it++) {
                    if (human->getMeld(it->getRank()) ||
                        human->canCreateMeld(*it)) //we do not want to discard cards that the human player has
                        continue;

                    if (it->getPoints() < lowestPoint) {
                        lowest = it;
                        lowestPoint = it->getPoints();
                    }
                }

                // means that we couldn't initially find a card to pick from
                if (lowest == ai->getHand()->end() && !tookDiscard) {

                    discardTop = discardPile->topCard();
                    bool humanCanUseDiscard = human->canCreateMeld(*discardTop);

                    // if the human can take from the discard pile, and we have a wildcard, freeze the discard pile
                    if (humanCanUseDiscard && ai->getHand()->getWildCards() > 0) {
                        for (auto it = ai->getHand()->begin(); it != ai->getHand()->end(); it++) {
                            if (it->isWildcard()) {
                                stream << "The AI decided to discard " << *lowest
                                       << " to freeze the discard pile and prevent you from using it this turn"
                                       << std::endl;
                                lowest = it;
                                break;
                            }
                        }
                    } else {
                        lowestPoint = 1000;
                        for (auto it = ai->getHand()->begin(); it != ai->getHand()->end(); it++) {
                            if (it->getPoints() < lowestPoint) {
                                lowestPoint = it->getPoints();
                                lowest = it;
                            }
                        }
                        stream << "The AI chose to discard the " << *lowest
                               << " because it's worth the least amount of points." << std::endl;
                    }
                } else if (lowest == ai->getHand()->end()) {
                    // means that we took from the discard pile, but every card in our hand is something the player can make a meld with.

                    // select card with the lowest point value
                    lowestPoint = 1000;
                    for (auto it = ai->getHand()->begin(); it != ai->getHand()->end(); it++) {
                        if (it->isBlackThree() || it->isWildcard())
                            continue;

                        if (it->getPoints() < lowestPoint) {
                            lowestPoint = it->getPoints();
                            lowest = it;
                        }
                    }

                    if (lowest != ai->getHand()->end()) {
                        stream << "The AI chose to discard the " << *lowest
                               << " because it's worth the least amount of points." << std::endl;
                    }
                } else
                    stream << "The AI chose to discard the " << *lowest
                           << " because you do not have any melds of it and it is worth the least amount of points."
                           << std::endl;

                discardPile->addCard(*lowest);
                ai->getHand()->removeCard(lowest);
            }
        } else {
            ai->setIsOut(true);
            std::cout << "The AI chose to go out." << std::endl;
            endGame();
        }

        // print stringstream to stdout after
        std::cout << stream.str() << std::endl;
    }

    void Game::canAddToMelds(Player *player, std::vector<int> &ret) {
        for (auto &m: player->getMelds()) {
            if (m) {

                //create a quick copy
                Meld copy(m->getRank());
                std::shared_ptr<Card> top = m->topCard();
                if (top && top->isRedThree())
                    copy = RedThreeMeld();
                else if (top && top->isBlackThree())
                    copy = BlackThreeMeld();

                for (auto &c: *m)
                    copy.addCard(c);

                for (auto &c: *player->getHand()) {
                    size_t tmp = copy.count();
                    copy.addCard(c);

                    if (tmp != copy.count())
                        ret.push_back(copy.getRank());
                }
            }
        }
    }

    bool Game::shouldStop() {
        if (stockPile->count() == 1 &&
            stockPile->topCard()->isRedThree()) //game always has to end if the top card of stockpile is red three
            return true;

        // otherwise if the stockpile is not empty then we can continue fine
        if (!stockPile->empty())
            return false;

        // if the top card on discard pile froze the pile, then game has to end
        std::shared_ptr<Card> top = discardPile->topCard();
        if (top->canFreezeDiscard())
            return true;

        Player *p1 = players[0];
        Player *p2 = players[1];

        // determine if either player can either create a meld with that card, or add it to an existing meld
        bool canUse1 = p1->canCreateMeld(*top) || p1->getMeld(top->getRank());
        bool canUse2 = p2->canCreateMeld(*top) || p1->getMeld(top->getRank());

        return canUse1 || canUse2;
    }

    void Game::endGame() {
        started = false;

        // set player points here forcefully
        int winner;
        int winnerPoints = 0;

        for (int i = 0; i < MAX_PLAYERS; i++) {
            const char *str = (i == 0 ? "Player" : "AI");

            Player *p = players[i];
            int points = p->getPoints();
            int earned = p->calculatePoints();
            p->setPoints(earned + points);

            if (points > winnerPoints) {
                winner = i;
                winnerPoints = p->getPoints();
            }

            std::cout << str << " final points: " << p->getPoints() << std::endl;
        }

        const char *str = (winner == 0 ? "Player" : "AI");
        std::cout << "Game over! " << str << " won!" << std::endl;
    }

    int Game::getCurrentPlayerIndex() {
        return currentPlayer;
    }
}

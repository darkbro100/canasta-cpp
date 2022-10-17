//
// Created by pauli on 9/9/2022.
//

#include "game.h"
#include "input_handler.h"
#include "game_serializer.h"
#include <random>
#include <algorithm>
#include <sstream>

/**
 * static function for this translation unit. Used for generating which player will play first during a coin toss
 * @return Player to move [0-1]
 */
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
        currentRound = 0;
        started = false;
        currentPlayer = -1;
        startingPlayer = -1;
        stockPile = new Deck();
        discardPile = new Deck(false);

        players = new Player *[MAX_PLAYERS];
        for (int i = 0; i < MAX_PLAYERS; i++) {
            players[i] = new Player();
            players[i]->createMeld(-1, true);
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
        currentRound = 1;

        for (int i = 0; i < MAX_PLAYERS; i++)
            players[i]->setPoints(0);

        // shuffle the deck a few times
        for (int i = 0; i < 3; i++)
            stockPile->shuffle();

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

        // sort both players hands (for looks, but also used when doing meld calculations, hand must be in a certain order)
        for (int i = 0; i < MAX_PLAYERS; i++)
            std::sort(players[i]->getHand()->begin(), players[i]->getHand()->end(), std::greater<Card>());

        std::shared_ptr<Card> c = stockPile->drawCard();
        discardPile->addCard(*c);

        while (c->isRedThree() || c->isWildcard()) {
            c = stockPile->drawCard();
            discardPile->addCard(*c);
        }
    }

    int Game::getCurrentRound() {
        return currentRound;
    }

    void Game::choosePlayer() {
        int p1 = players[HUMAN_PLAYER]->getPoints();
        int p2 = players[CPU_PLAYER]->getPoints();

        if (p1 > p2) {
            currentPlayer = HUMAN_PLAYER;
            startingPlayer = currentPlayer;
        } else if (p2 > p1) {
            currentPlayer = CPU_PLAYER;
            startingPlayer = currentPlayer;
        } else {
            int toss = displayCoinToss();
            int chosen = randomPlayer();
            std::cout << "You chose " << (toss == 0 ? "heads" : "tails") << " and " << (toss == chosen ? "won" : "lost")
                      << " the coin toss!" << std::endl;
            currentPlayer = toss == chosen ? HUMAN_PLAYER : CPU_PLAYER;
            startingPlayer = currentPlayer;
        }
    }

    Player *Game::getCurrentPlayer() {
        return players[currentPlayer];
    }

    void Game::start() {
        started = true;
    }

    void Game::startTurn() {
        // if the game cannot progress any further, then we stop it here
        if (shouldStop()) {
            std::cout
                    << "The round is ending because it is in a state where both players can no longer interact with the stock/discard piles."
                    << std::endl;
            endGame();
            return;
        }

        // Display pre-information
        PreCommands code = displayPreRound();
        switch (code) {
            case SAVE:
                std::cout << "Saving the game to ./game_dump.txt ..." << std::endl;
                saveGame("./game_dump.txt", *this);
                started = false;
                return;
            case HELP:
                std::cout << "Displaying help menu..." << std::endl;
                helpMenu();
                return;
            case QUIT:
                started = false;
                return;
            case TAKE:
                break;
            default:
                std::cout << "Unknown command..." << std::endl;
                return;
        }

        // if the starting/current player has not been initialized, ensure we choose a player
        if (startingPlayer == -1 && currentPlayer == -1)
            choosePlayer();

        Player *p = this->getCurrentPlayer();

        // display information about this current turn
        std::cout << "Current Round: " << currentRound << std::endl;
        std::cout << "Your Hand: " << *players[0]->getHand() << std::endl;
        std::cout << "CPU Hand: " << *players[1]->getHand() << std::endl << std::endl;

        std::cout << "Your Melds: ";
        for (auto &it: players[0]->getMelds()) {
            if (it->empty()) // the melds for both 3s will show up by default
                continue;
            std::cout << "[" << *it << "] ";
        }
        std::cout << std::endl;

        std::cout << "CPU Melds: ";
        for (auto &it: players[1]->getMelds()) {
            if (it->empty()) // the melds for both 3s will show up by default
                continue;
            std::cout << "[" << *it << "] ";
        }
        std::cout << std::endl << std::endl;

        // only display stockpile and discard pile if they aren't empty
        if (!stockPile->empty())
            std::cout << "Stockpile (top): " << *stockPile->topCard() << " + " << stockPile->count() - 1 << " more"
                      << std::endl;

        if (!discardPile->empty())
            std::cout << "Discard (top): " << *discardPile->topCard() << " + " << discardPile->count() - 1 << " more"
                      << std::endl
                      << std::endl; // additional endl for a line space

        //Do turn checking here
        if (currentPlayer == CPU_PLAYER) {

            //calculate what the cpu should do here
            aiTurn();

        } else { // Human player

            //Check if player can go out and ask them if they want to
            if (p->canGoOut()) {
                int shouldQuit = displayShouldGoOut();
                p->setIsOut(shouldQuit == 1);
                if (p->isPlayerOut()) {
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
            currentPlayer = 0; //reset to beginning of array

        //none of this code is needed as i misinterpreted how rounds and turns worked (ref: during initial demo)
//        if (currentPlayer == startingPlayer) {
//            currentRound++;
//            std::cout << "==== ROUND OVER ====" << std::endl;
//            for (int i = 0; i < MAX_PLAYERS; i++) {
//                p = players[i];
//
//                int points = p->getPoints();
//                int earned = p->calculatePoints();
//                p->setPoints(earned + points);
//
//                const char *str = (i == HUMAN_PLAYER ? "Player" : "AI");
//                std::cout << str << " earned " << earned << " points this round." << std::endl;
//                std::cout << str << " earned " << p->getPoints() << " points in total." << std::endl;
//                std::cout << std::endl;
//            }
//            std::cout << std::endl;
//            choosePlayer();
//        }
    }

    bool Game::isStarted() {
        return started;
    }

    /**
     * This assumes that the player is the human player
     */
    void Game::drawTurn(Player *p) {
        std::shared_ptr<Card> discardTop = discardPile->topCard();
        Meld *canAddToMeld = discardTop ? p->getMeld(discardTop->getRank()) : nullptr;
        bool canUseDiscard = discardTop && p->canCreateMeld(*discardTop);

        if (discardTop && !discardTop->canFreezeDiscard() && (canAddToMeld || canUseDiscard)) {
            DrawCommands command = displayDrawOptions();

            if (command == DrawCommands::DRAW_HELP) {
                helpMenu(); // display help menu, recursive call, then return
                drawTurn(p);
                return;
            } else if (command == DrawCommands::STOCK) {
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
                return;
            }
        }

        draw:
        size_t oldSize = p->getHand()->count();

        bool shouldEnd = p->drawCard(stockPile, false);
        if (shouldEnd) {
            endGame();
            return;
        }
        size_t newSize = p->getHand()->count();
        size_t difference = newSize - oldSize;

        std::cout << "You drew: ";
        for (size_t i = 0; i < difference; i++)
            std::cout << p->getHand()->operator[](i) << " ";
        std::cout << std::endl;
    }

    void Game::meldTurn(Player *p) {
        int meld = p->canCreateMeld();
        int addToMeld = p->checkMeld();

        // determine what the player wants to do with melds. if they choose to do nothing or can't do anything, stop here
        MeldCommands cmd;
        if (meld == -1 && addToMeld == -1)
            cmd = MeldCommands::NONE;
        else
            cmd = displayMeldOptions(meld != -1, addToMeld != -1);

        // if the player still chose to do nothing for some reason, then stop
        if (cmd == MeldCommands::NONE)
            return;

        switch (cmd) {
            case MELD_HELP: {
                helpMenu(); // display normal help menu
                meldTurn(p);
                return;
            }
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
            Meld *canAddToMeld = discardTop ? ai->getMeld(discardTop->getRank()) : nullptr;
            bool canUseDiscard = discardTop && ai->canCreateMeld(*discardTop);

            /**
             * Deciding what to do with drawing
             *
             * We don't do calculations for stockpile
             * because it is technically "face down"
             */
            // if the discard pile is not frozen, and we can do something with it
            if (discardTop && !discardTop->canFreezeDiscard() && (canAddToMeld || canUseDiscard)) {

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
                size_t oldSize = ai->getHand()->count();
                bool shouldEnd = ai->drawCard(stockPile);
                if (shouldEnd) {
                    endGame();
                    return;
                }
                size_t newSize = ai->getHand()->count();
                size_t diff = newSize - oldSize;

                std::cout << "The AI could not use the discard pile and drew: ";
                for(size_t i = 0; i < diff; i++)
                    std::cout << ai->getHand()->operator[](i) << " ";
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
            ai->canAddToMelds(melds);

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
                    bool humanCanUseDiscard = !(discardTop == nullptr) && human->canCreateMeld(*discardTop);

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

    bool Game::shouldStop() {
        //game always has to end if the stockpile contains *only* a red three
        if (stockPile->count() == 1 && stockPile->topCard()->isRedThree())
            return true;

        // if the stockpile is empty and the discard pile is also empty (meaning we can't use the discard pile), then return true
        if (stockPile->empty() && discardPile->empty())
            return true;

        // if only the stockpile is empty we need to check if the game can continue
        if (stockPile->empty()) {
            // if the top card on discard pile froze the pile, then game has to end
            std::shared_ptr<Card> top = discardPile->topCard();
            if (!top ||
                top->canFreezeDiscard()) //if the top card doesn't exist, or it can freeze the discard pile then the game must stop
                return true;

            Player *p1 = players[HUMAN_PLAYER];
            Player *p2 = players[CPU_PLAYER];

            // determine if either player can either create a meld with that card, or add it to an existing meld
            bool canUse1 = p1->canCreateMeld(*top) || p1->getMeld(top->getRank()) != nullptr;
            bool canUse2 = p2->canCreateMeld(*top) || p1->getMeld(top->getRank()) != nullptr;

            return !canUse1 && !canUse2;
        }

        // otherwise the game can continue
        return false;
    }

    void Game::endGame() {
        // define some stuff for determining who the winner is
        int winner;
        int winnerPoints = 0;

        for (int i = 0; i < MAX_PLAYERS; i++) {
            const char *str = (i == HUMAN_PLAYER ? "Player" : "AI");

            Player *p = players[i];
            int points = p->getPoints();
            int total = points + p->calculatePoints();
            p->setPoints(total);

            if (total > winnerPoints) {
                winner = i;
                winnerPoints = total;
            }

            std::cout << str << " points: " << p->getPoints() << std::endl;
        }

        int shouldRestart = displayRoundRestart();
        if (!shouldRestart) {
            const char *str = (winner == HUMAN_PLAYER ? "Player" : "AI");
            std::cout << "Game over! " << str << " won!" << std::endl;
            started = false;
        } else {
            int tmp = currentRound + 1; // increment round counter
            std::cout << "Setting up new round..." << std::endl;

            // store points
            int humanPoints = players[HUMAN_PLAYER]->getPoints();
            int cpuPoints = players[CPU_PLAYER]->getPoints();

            // ensure who goes first next round actually happens
            startingPlayer = -1;
            currentPlayer = -1;

            // delete old allocated memory
            delete stockPile;
            delete discardPile;
            delete[] players;

            // reassign everything
            stockPile = new Deck();
            discardPile = new Deck(false);

            players = new Player *[MAX_PLAYERS];
            for (int i = 0; i < MAX_PLAYERS; i++) {
                players[i] = new Player();
                players[i]->createMeld(-1, true);
            }

            // call setup
            setup();
            currentRound = tmp; // quick work around because calling #setup will reset currentRound back to 1

            // ensure points are copied over
            for (int i = 0; i < MAX_PLAYERS; i++) {
                // ensure points are copied over
                if (i == CPU_PLAYER)
                    players[i]->setPoints(cpuPoints);
                else
                    players[i]->setPoints(humanPoints);
            }
        }
    }

    int Game::getCurrentPlayerIndex() {
        return currentPlayer;
    }

    void Game::setRound(int turn) {
        this->currentRound = turn;
    }

    void Game::setCurrentPlayerIndex(int index) {
        this->currentPlayer = index;
    }

    void Game::helpMenu() {

        // tell the player they can only ask for help if it is their turn
        if (getCurrentPlayerIndex() == CPU_PLAYER) {
            std::cout << "It is the AIs turn right now! Wait until your turn!" << std::endl;
            return;
        }

        Player *p = getPlayer(HUMAN_PLAYER);
        Player *ai = getPlayer(CPU_PLAYER); // Get AI player

        std::stringstream stream;

        bool tookDiscard = false;

        std::shared_ptr<Card> discardTop = discardPile->topCard();
        Meld *canAddToMeld = discardTop == nullptr ? nullptr : p->getMeld(discardTop->getRank());
        bool canUseDiscard = discardTop && p->canCreateMeld(*discardTop);

        // if the discard pile is not frozen, and we can do something with it
        if (discardTop && !discardTop->canFreezeDiscard() && (canAddToMeld || canUseDiscard)) {

            // usually if you can interact with the discard pile in any kind of way it is beneficial
            if (canAddToMeld) {
                stream << "You can use the discard pile to add to meld: " << *canAddToMeld << std::endl;
            } else {
                stream << "You can use the discard pile to create a new meld entirely" << std::endl;
            }

            tookDiscard = true;
        } else {
            stream << "You cannot take from the discard pile, so you must draw from the stockpile." << std::endl;
        }

        // checks what meld the player can make
        int canMake = p->canCreateMeld();
        if (canMake != -1) {

            // construct temporary meld to display to stdout
            Meld m(canMake);
            if (canMake == 3)
                m = BlackThreeMeld();

            for (auto &c: *p->getHand())
                m.addCard(c);

            stream << "You can create a meld: " << m << std::endl;
        }

        // checks what melds the player can add to
        std::vector<int> melds;
        p->canAddToMelds(melds);

        // erase duplicate values
        std::sort(melds.begin(), melds.end());
        melds.erase(std::unique(melds.begin(), melds.end()), melds.end());

        // format melds to strstream
        if (!melds.empty()) {
            stream << "You can add to melds: ";

            for (auto it = melds.begin(); it != melds.end(); it++) {
                if (it != melds.begin())
                    stream << ", ";

                Meld *m = *it == 3 ? p->getBlackThreeMeld() : p->getMeld(*it);
                stream << "[ " << *m << " ] ";
            }
            stream << std::endl;
        }

        if (p->getHand()->empty()) {
            stream << "You cannot add a card to the discard pile" << std::endl;
        } else {
            std::vector<Card>::iterator lowest = p->getHand()->end();
            int lowestPoint = 1000;

            for (auto it = p->getHand()->begin(); it != p->getHand()->end(); it++) {
                if (ai->getMeld(it->getRank()) ||
                    ai->canCreateMeld(*it)) //we do not want to discard cards that the human player has
                    continue;

                if (it->getPoints() < lowestPoint) {
                    lowest = it;
                    lowestPoint = it->getPoints();
                }
            }

            // means that we couldn't initially find a card to pick from
            if (lowest == p->getHand()->end() && !tookDiscard) {

                discardTop = discardPile->topCard();
                bool aiCanUse = !(discardTop == nullptr) && ai->canCreateMeld(*discardTop);

                // if the human can take from the discard pile, and we have a wildcard, freeze the discard pile
                if (aiCanUse && p->getHand()->getWildCards() > 0) {
                    for (auto &it: *p->getHand()) {
                        if (it.isWildcard()) {
                            stream << "Discard the " << it
                                   << " to freeze the discard pile and prevent the AI from using it next turn"
                                   << std::endl;
                            break;
                        }
                    }
                } else {
                    lowestPoint = 1000;
                    for (auto it = p->getHand()->begin(); it != p->getHand()->end(); it++) {
                        if (it->getPoints() < lowestPoint) {
                            lowestPoint = it->getPoints();
                            lowest = it;
                        }
                    }
                    stream << "Discard the " << *lowest
                           << " because it's worth the least amount of points." << std::endl;
                }
            } else if (lowest == p->getHand()->end()) {
                // means that we took from the discard pile, but every card in our hand is something the player can make a meld with.

                // select card with the lowest point value
                lowestPoint = 1000;
                for (auto it = p->getHand()->begin(); it != p->getHand()->end(); it++) {
                    if (it->isBlackThree() || it->isWildcard())
                        continue;

                    if (it->getPoints() < lowestPoint) {
                        lowestPoint = it->getPoints();
                        lowest = it;
                    }
                }

                if (lowest != p->getHand()->end()) {
                    stream << "Discard the " << *lowest
                           << " because it's worth the least amount of points." << std::endl;
                }
            } else
                stream << "Discard the " << *lowest
                       << " because the AI does not have any melds of it and it is worth the least amount of points."
                       << std::endl;

        }

        // if the player can go out, suggest what they should do
        if (p->canGoOut()) {
            if (p->getPoints() > ai->getPoints())
                stream << "You can go out this turn and you should since you would win the game!" << std::endl;
            else
                stream << "You can go out this turn, but you would lose if you did." << std::endl;
        }

        std::cout << stream.str() << std::endl;
    }
}

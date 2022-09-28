//
// Created by pauli on 9/14/2022.
//

#include "game_serializer.h"
#include <cstring>
#include <fstream>

namespace Canasta {

    static Card parseCard(std::string &str) {
        int rank;

        // edge case for jokers, just return a joker value
        if (strcmp(str.c_str(), "Joker") == 0)
            return {Suit::CLUBS, -1};

        bool ten = strcmp(str.substr(0, 2).c_str(), "10") == 0;

        char c = str[0];
        char s = str[ten ? 2 : 1];

        Suit suit;

        // if we are a ten, then just set the rank here since 10 requires 2 chars as opposed to 1
        if (ten) {
            rank = 10;
        } else {
            switch (c) {
                case 'X':
                    rank = 10;
                    break;
                case 'J':
                    rank = 11;
                    break;
                case 'Q':
                    rank = 12;
                    break;
                case 'K':
                    rank = 13;
                    break;
                case 'A':
                    rank = 1;
                    break;
                default:
                    rank = (int) c - '0';
                    break;
            }
        }

        switch (s) {
            case 'C':
                suit = Suit::CLUBS;
                break;
            case 'H':
                suit = Suit::HEARTS;
                break;
            case 'S':
                suit = Suit::SPADES;
                break;
            case 'D':
                suit = Suit::DIAMONDS;
                break;
            case '1':
            case '2':
                if (c == 'J')
                    return {Suit::CLUBS, -1}; // edge case for how jokers are stored in kumar's serialization files.
            default:
                suit = Suit::CLUBS;
                break;
        }

        return {suit, rank};
    }

    static const char *matches(const std::string &str) {
        for (auto &i: SERIALIZATION_KEYS)
            if (strcmp(str.c_str(), i) == 0)
                return i;

        return nullptr;
    }

    void serializeGame(Game &game, std::ostream &stream) {
        int turn = game.getCurrentTurn();
        stream << "Round: " << turn << "\n\n";

        for (int i = MAX_PLAYERS - 1; i >= 0; i--) {
            Player *p = game.getPlayer(i);
            const char *str = i == CPU_PLAYER ? "Computer" : "Human";

            stream << str << ":" << std::endl;
            stream << "\tScore: " << p->getPoints() << std::endl;
            stream << "\tHand: " << *p->getHand() << std::endl;
            stream << "\tMelds: ";

            for (auto &it: p->getMelds()) {
                if (it->empty())
                    continue;
                stream << "[" << *it << "] ";
            }

            stream << "\n\n";
        }

        stream << "Stock: " << *game.getStockPile() << std::endl;
        stream << "Discard Pile: " << *game.getDiscardPile() << std::endl;

        const char *str = game.getCurrentPlayerIndex() == CPU_PLAYER ? "Computer" : "Human";
        stream << "Next Player: " << str;
    }

    void deserializeGame(Game &game, std::istream &stream) {

        const char *lastKey = nullptr;
        bool scanNext = false;
        Player *modifyingPlayer = nullptr;
        Meld *modifyingMeld = nullptr;

        do {
            std::string test;
            stream >> test;

            const char *key = matches(test);
            if (key) {
                // we are modifying CPU data for the game
                if (key == SERIALIZATION_KEYS[1]) {
                    modifyingPlayer = game.getPlayer(CPU_PLAYER);
                } else if (key == SERIALIZATION_KEYS[2]) {
                    // we are modifying PLAYER data for the game
                    modifyingPlayer = game.getPlayer(HUMAN_PLAYER);
                } else {
                    lastKey = key;
                    scanNext = true;
                }

            } else if (scanNext) {
                if (strcmp(lastKey, "Score:") == 0) {
                    try {
                        int score = std::stoi(test);
                        modifyingPlayer->setPoints(score);
                    }catch(std::invalid_argument const & e) { } // don't need to print the exception, if stoi throws an exception it means that the input provided for the round/score was invalid
                } else if (strcmp(lastKey, "Round:") == 0) {
                    try {
                        int round = std::stoi(test);
                        game.setTurn(round);
                    }catch(std::invalid_argument const & e) { } // don't need to print the exception, if stoi throws an exception it means that the input provided for the round/score was invalid
                } else if (strcmp(lastKey, "Hand:") == 0) {
                    Deck *hand = modifyingPlayer->getHand();
                    Card c = parseCard(test);
                    hand->addCard(c);

                    scanNext = true;
                } else if (strcmp(lastKey, "Melds:") == 0) {

                    // remove end brackets if needed
                    if (test[test.size() - 1] == ']')
                        test = test.substr(0, test.size() - 1);

                    // if start bracket that means a new meld has begun
                    if (test[0] == '[') {
                        std::string cardStr = test.substr(1, test.size());
                        Card parsed = parseCard(cardStr);

                        modifyingMeld = modifyingPlayer->createMeld(parsed.getRank(), parsed.isRedThree(),
                                                                    parsed.isBlackThree());
                        modifyingMeld->addCard(parsed);
                    } else {
                        Card parsed = parseCard(test);
                        modifyingMeld->addCard(parsed);
                    }

                    scanNext = true;
                } else if (strcmp(lastKey, "Stock:") == 0) {
                    Deck *stock = game.getStockPile();
                    Card c = parseCard(test);
                    stock->addCard(c);
                } else if (strcmp(lastKey, "Pile:") == 0) {
                    Deck *discard = game.getDiscardPile();
                    Card c = parseCard(test);
                    discard->addCard(c);
                } else if (strcmp(lastKey, "Player:") == 0) {
                    const char *cstr = test.c_str();
                    if (strcmp(cstr, "Human") == 0)
                        game.setCurrentPlayerIndex(0);
                    else
                        game.setCurrentPlayerIndex(1);

                    scanNext = false;
                }

            }

        } while (stream);
    }

    void loadGame(const char *file, Game &game) {
        std::ifstream stream;
        stream.open(file);

        deserializeGame(game, stream);
        stream.close();
    }

    void saveGame(const char *file, Game &game) {
        std::ofstream stream;
        stream.open(file);

        serializeGame(game, stream);
        stream.close();
    }
}
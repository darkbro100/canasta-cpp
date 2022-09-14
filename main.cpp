#include <iostream>
#include "game.h"
#include "game_serializer.h"

using namespace Canasta;

/**
 * TODO:
 * - Add help menu
 * - Add in serialization of games
 * - Add game saving (ties into serialization)
 */

/**
 * TODO: not sure if this will stay the same. dont know if i want to move the keyboard input into its own class/file or keep it how the way it is and pass the char code to the game object and have the game object handle the specific input there
 */
static char code;
static void handleInput() {
    std::cin >> code;
    code = (char) (std::tolower(code));
}

int main() {
    std::stringstream  stream;

    Game g;
    g.setup();

    Game copy;
    copy.getStockPile()->clear();

    Player * p = g.getCurrentPlayer();

    Meld * m = p->createMeld(5);
    m->addCard(Card(Suit::CLUBS, 5));
    m->addCard(Card(Suit::CLUBS, 5));
    m->addCard(Card(Suit::CLUBS, 5));

    m = p->createMeld(7);
    m->addCard(Card(Suit::CLUBS, 7));
    m->addCard(Card(Suit::CLUBS, 7));
    m->addCard(Card(Suit::CLUBS, 7));
    m->addCard(Card(Suit::CLUBS, 7));

    m = p->getRedThreeMeld();
    m->addCard(Card(Suit::HEARTS, 3));

    serializeGame(g, stream);
    std::cout << "SERIALIZED GAME:" << std::endl;
    std::cout << stream.str() << std::endl;

    std::istringstream  istream(stream.str());
    deserializeGame(copy, istream);
    std::cout << "DESERIALIZED GAME:" << std::endl;

    std::stringstream nstream;
    serializeGame(copy, nstream);
    std::cout << nstream.str() << std::endl;

//    printf("Game setup!\n\nHit [y] to start, anything else to quit.\n");
//
//    handleInput();
//    if (code == 'y') {
//        g.start();
//        while (g.isStarted()) {
//            g.startTurn();
//        }
//    } else {
//        std::cout << "Exiting..." << std::endl;
//        exit(0);
//    }
}
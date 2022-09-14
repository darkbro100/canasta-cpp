//
// Created by pauli on 9/14/2022.
//

#ifndef CANASTA_CPP_GAME_SERIALIZER_H
#define CANASTA_CPP_GAME_SERIALIZER_H

#include <sstream>
#include "game.h"

namespace Canasta {

    /**
     * Store an array of all the "keys" that are unique to the serialization process
     */
     static const int SERIALIZATION_LENGTH = 11;
    static const char * SERIALIZATION_KEYS[SERIALIZATION_LENGTH] = {
      "Round:",
      "Computer:",
      "Human:",
      "Score:",
      "Hand:",
      "Melds:",
      "Stock:",
      "Discard",
      "Pile:",
      "Next",
      "Player:"
    };

    void serializeGame(Game & game, std::stringstream & stream);

    /**
     * Deserializes an istringstream into the refereneced game object
     * @param game  Game object to update
     * @param stream Stream containing serialized game
     */
    void deserializeGame(Game & game, std::istringstream & stream);

    void readFile(const char * file, Game & game);
    void writeFile(const char * file, Game & game);

}


#endif //CANASTA_CPP_GAME_SERIALIZER_H

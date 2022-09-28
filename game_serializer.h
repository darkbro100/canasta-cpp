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
    static const char *SERIALIZATION_KEYS[SERIALIZATION_LENGTH] = {
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

    /**
     * Serializes a referenced Game object to the given istream
     * @param game  Reference to game object
     * @param stream Reference to istream to serialize to
     */
    void serializeGame(Game &game, std::ostream &stream);

    /**
     * Deserializes a referenced Game object from the given istream
     * @param game  Reference to game object
     * @param stream Reference to istream to deserialize from
     */
    void deserializeGame(Game &game, std::istream &stream);

    /**
     * Reads a game's data from a given file, and applies it to a given game
     * @param file File to read from
     * @param game Game to update
     */
    void loadGame(const char *file, Game &game);

    /**
     * Writes a game's data to a given file.
     * @param file File to write to
     * @param game Game reference
     */
    void saveGame(const char *file, Game &game);

}


#endif //CANASTA_CPP_GAME_SERIALIZER_H

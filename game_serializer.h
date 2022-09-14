//
// Created by pauli on 9/14/2022.
//

#ifndef CANASTA_CPP_GAME_SERIALIZER_H
#define CANASTA_CPP_GAME_SERIALIZER_H

#include <sstream>
#include "game.h"

namespace Canasta {
    void serializeGame(Game & game, std::stringstream & stream);
}


#endif //CANASTA_CPP_GAME_SERIALIZER_H

//
// Created by pauli on 9/9/2022.
//

#ifndef CANASTA_CPP_PLAYER_H
#define CANASTA_CPP_PLAYER_H

#include "deck.h"

namespace Canasta {
    class Player {
    private:
        Deck * hand;
    public:
        Player();
        ~Player();
        Player(Player & p);
        Player(Player && p) noexcept;

        Deck * getHand();
    };
}

#endif //CANASTA_CPP_PLAYER_H

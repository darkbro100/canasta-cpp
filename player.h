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
        int points;
    public:
        Player();
        ~Player();
        Player(Player & p);
        Player(Player && p) noexcept;
        int getPoints();
        void setPoints(int points);

        Deck * getHand();
    };
}

#endif //CANASTA_CPP_PLAYER_H

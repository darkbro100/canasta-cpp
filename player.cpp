//
// Created by pauli on 9/9/2022.
//

#include "player.h"

namespace Canasta {
    Deck * Player::getHand() {
        return hand;
    }

    Player::Player() {
        hand = new Deck(false);
    }

    Player::Player(Player &p) {
        std::cout << "copy constructor [player]" << std::endl;
        this->hand = new Deck(false);
        for(auto & it : *p.hand) {
            this->hand->addCard(it);
        }
    }

    Player::Player(Player &&p)  noexcept {
        std::cout << "move constructor [player]" << std::endl;
        this->hand = p.hand;
        p.hand = nullptr;
    }

    Player::~Player() {
        delete hand;
    }
}
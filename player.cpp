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
        this->hand = new Deck(false);
        for(auto & it : *p.hand) {
            this->hand->addCard(it);
        }
    }

    Player::Player(Player &&p)  noexcept {
        this->hand = p.hand;
        p.hand = nullptr;
    }

    Player::~Player() {
        delete hand;
    }

    void Player::setPoints(int points) {
        this->points = points;
    }

    int Player::getPoints() {
        return points;
    }
}
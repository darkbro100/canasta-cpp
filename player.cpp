//
// Created by pauli on 9/9/2022.
//

#include "player.h"
#include <algorithm>

namespace Canasta {
    Deck *Player::getHand() {
        return hand;
    }

    Player::Player() {
        hand = new Deck(false);
        points = 0;
        melds = std::vector<Meld*>();
    }

    Player::Player(Player &p) {
        // remove existing melds
        for (auto &meld: melds)
            delete meld;
        melds.clear();

        this->points = p.points;
        this->hand = new Deck(false);

        for (auto &it: *p.hand) {
            this->hand->addCard(it);
        }

        //copy melds over
        for (auto &meld: p.melds) {
            Meld * copy = new Meld(meld->getRank());
            for(auto & card : *meld)
                copy->addCard(card);
            this->melds.push_back(copy);
        }
    }

    Player::Player(Player &&p) noexcept {
        // remove existing melds
        for (auto &meld: melds)
            delete meld;
        melds.clear();

        this->hand = p.hand;
        this->points = p.points;

        //move melds over
        for (auto &meld: p.melds)
            this->melds.push_back(meld);

        p.points = 0;
        p.hand = nullptr;
        p.melds.clear();
    }

    Player::~Player() {
        for (auto &meld: melds) delete meld;
        delete hand;
    }

    void Player::setPoints(int points) {
        this->points = points;
    }

    int Player::getPoints() {
        return points;
    }

    Meld *Player::getMeld(int rank) {
        for (auto &meld: melds) {
            if (meld && meld->getRank() == rank)
                return meld;
        }

        return nullptr;
    }

    Meld *Player::createMeld(int rank) {
        Meld *exists = getMeld(rank);
        if (exists)
            return exists;

        exists = new Meld(rank);
        melds.push_back(exists);

        return exists;
    }

    bool Player::canCreateMeld(Card card) {
        // create meld on the stack
        Meld m(card.getRank());
        m.addCard(card);

        for (auto &it: *hand)
            m.addCard(it);

        std::cout << "attempted meld: " << m << std::endl;
        return m.count() >= 3;
    }

    int Player::canCreateMeld() {
        for (auto c1 = hand->begin(); c1 != hand->end(); c1++) {
            Meld m(c1->getRank());
            m.addCard(*c1);

            for (auto c2 = hand->begin(); c2 != hand->end(); c2++) {
                if (c1 == c2)
                    continue;

                m.addCard(*c2);
            }

            if(m.count() >= 3)
                return m.getRank();
        }

        return -1;
    }
}
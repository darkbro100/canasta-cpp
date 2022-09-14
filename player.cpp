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
        isOut = false;
        hand = new Deck(false);
        points = 0;
        melds = std::vector<Meld *>();
    }

    Player::Player(Player &p) {
        // remove existing melds
        isOut = p.isOut;

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
            Meld *copy = new Meld(meld->getRank());
            for (auto &card: *meld)
                copy->addCard(card);
            this->melds.push_back(copy);
        }
    }

    Player::Player(Player &&p) noexcept {
        // remove existing melds
        for (auto &meld: melds)
            delete meld;
        melds.clear();

        this->isOut = p.isOut;
        this->hand = p.hand;
        this->points = p.points;

        //move melds over
        for (auto &meld: p.melds)
            this->melds.push_back(meld);

        p.isOut = false;
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

    Meld *Player::createMeld(int rank, bool redThree, bool blackThree) {
        Meld *exists = nullptr;

        if (redThree)
            exists = getRedThreeMeld();
        else if (blackThree)
            exists = getBlackThreeMeld();
        else
            exists = getMeld(rank);

        if (exists)
            return exists;

        if (redThree)
            exists = new RedThreeMeld();
        else if (blackThree)
            exists = new BlackThreeMeld();
        else
            exists = new Meld(rank);

        melds.push_back(exists);
        return exists;
    }

    bool Player::canCreateMeld(Card card) {
        // create meld on the stack
        Meld m(card.getRank());
        m.addCard(card);

        // we need to sort here because of how we check for melds, jokers need to be last in the iteration so it doesn't mess up meld calculations
        std::sort(hand->begin(), hand->end(), std::greater<Card>());
        for (auto &it: *hand)
            m.addCard(it);

        return m.count() >= MELD_COUNT;
    }

    int Player::canCreateMeld() {

        // we need to sort here because of how we check for melds, jokers need to be last in the iteration so it doesn't mess up meld calculations
        std::sort(hand->begin(), hand->end(), std::greater<Card>());
        for (auto c1 = hand->begin(); c1 != hand->end(); c1++) {
            if (c1->isBlackThree() && getBlackThreeMeld())
                continue;
            if (c1->isRedThree())
                continue;

            Meld m = c1->isBlackThree() ? BlackThreeMeld() : Meld(c1->getRank());
            m.addCard(*c1);

            for (auto c2 = hand->begin(); c2 != hand->end(); c2++) {
                if (c1 == c2)
                    continue;

                m.addCard(*c2);
            }

            if (m.count() >= MELD_COUNT)
                return m.getRank();
        }

        return -1;
    }

    int Player::checkMeld() {
        for (auto &m: melds) {
            if (m->getRank() == 3) continue; // skip red and black threes

            for (auto &card: *hand) {
                if (card.isWildcard() || m->getRank() == card.getRank()) {
                    return m->getRank();
                }
            }
        }

        return -1;
    }

    bool Player::drawCard(Deck *deck, bool takeAll) {
        if (takeAll) {
            while (!deck->empty()) {
                std::shared_ptr<Card> c = deck->drawCard();
                if (c->isRedThree())
                    getRedThreeMeld()->addCard(*c);
                else
                    hand->addCard(*c);
            }
        } else {
            std::shared_ptr<Card> c = deck->drawCard();
            while (c->isRedThree()) {
                if (deck->empty())
                    return true;

                getRedThreeMeld()->addCard(*c);
                c = deck->drawCard();
            }

            hand->addCard(*c);
        }

        return false;
    }

    std::vector<Meld *> &Player::getMelds() {
        return melds;
    }

    Meld *Player::getRedThreeMeld() {
        for (auto &meld: melds) {
            auto *redMeld = dynamic_cast<RedThreeMeld *>(meld);
            if (redMeld) return redMeld;
        }

        return nullptr;
    }

    Meld *Player::getBlackThreeMeld() {
        for (auto &meld: melds) {
            auto *blackMeld = dynamic_cast<BlackThreeMeld *>(meld);
            if (blackMeld) return blackMeld;
        }

        return nullptr;
    }

    bool Player::canGoOut() {
        bool hasCanasta = false;
        for (auto &m: melds) {
            if (m && m->isCanasta()) {
                hasCanasta = true;
                break;
            }
        }

        // If we don't have a canasta, we cannot go out
        if (!hasCanasta)
            return false;

        // If the player doesn't have anything in their hand, they can go out
        return hand->empty();
    }

    int Player::calculatePoints() {
        int points = 0;

        if (isOut)
            points += 100;

        for (auto &m: melds) {
            if (!m)
                continue;

            if (m->isNaturalCanasta())
                points += 500;
            else if (m->isCanasta())
                points += 300;

            for (auto &c: *m) {
                points += c.getPoints();
            }
        }

        for (auto &c: *hand)
            points -= c.getPoints();

        return points;
    }

    void Player::setIsOut(bool out) {
        this->isOut = out;
    }

    bool Player::isPlayerOut() {
        return this->isOut;
    }
}
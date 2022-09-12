//
// Created by pauli on 9/11/2022.
//

#include "meld.h"

namespace Canasta {
    int Canasta::Meld::getRank() {
        return rank;
    }

    void Canasta::Meld::addCard(Canasta::Card card) {
        int wildCards = getWildCards();
        size_t naturalCards = count() - wildCards;

        // cannot have more than 3 wildcards
        if (card.isWildcard() && getWildCards() >= 3)
            return;

        // can only use wildcards if we have atleast 2 natural cards
        if (card.isWildcard() && naturalCards < 2)
            return;

        // must either be a wildcard or a natural card with the same rank
        if (!card.isWildcard() && card.getRank() != rank)
            return;

        Deck::addCard(card);
    }

    bool Canasta::Meld::isCanasta() {
        return count() >= CANASTA_COUNT;
    }

    void Canasta::RedThreeMeld::addCard(Canasta::Card card) {
        if (!card.isRedThree())
            return;

        Meld::addCard(card);
    }

    void BlackThreeMeld::addCard(Card card) {
        if (!card.isBlackThree())
            return;

        Meld::addCard(card);
    }
}
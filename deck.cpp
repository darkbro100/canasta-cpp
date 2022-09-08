#include "deck.h"
#include <memory>

namespace Canasta {

    void Deck::addCard(const Card & card) {
        cards.push(card);
    }

    /**
     * I do not like currently how this is being done, feel like there is a way to do this without having to alloc something to the heap
     */
    std::shared_ptr<Card> Deck::drawCard() {
        std::shared_ptr<Card> ptr = std::make_shared<Card>(cards.top());
        cards.pop();

        return ptr;
    }

    bool Deck::empty() {
        return cards.empty();
    }
};

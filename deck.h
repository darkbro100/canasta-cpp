#pragma once
#include <stack>
#include "card.h"
#include <memory>

namespace Canasta {
class Deck {
private:
    std::stack<Card> cards;
public:
    void addCard(const Card & card);
    std::shared_ptr<Card> drawCard();
    bool empty();
};
};

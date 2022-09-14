//
// Created by pauli on 9/14/2022.
//

#include "game_serializer.h"

namespace Canasta {
    void serializeGame(Game &game, std::stringstream &stream) {
        int turn = game.getCurrentTurn();
        stream << "Round: " << turn << "\n\n";

        for(int i = MAX_PLAYERS - 1; i >= 0; i--) {
            Player * p = game.getPlayer(i);
            const char * str = i == 1 ? "Computer" : "Human";

            stream << str << ":" << std::endl;
            stream << "\tScore: " << p->getPoints() << std::endl;
            stream << "\tHand: " << *p->getHand() << std::endl;
            stream << "\tMelds: ";

            for(auto & it : p->getMelds()) {
                if(it->empty())
                    continue;
                stream << "[" << *it << "] ";
            }

            stream << "\n\n";
        }

        stream << "Stock: " << *game.getStockPile() << std::endl;
        stream << "Discard Pile: " << *game.getDiscardPile() << std::endl;

        const char * str = game.getCurrentPlayerIndex() == 1 ? "Computer" : "Human";
        stream << "Next Player: " << str << std::endl;
    }
}
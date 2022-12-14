//
// Created by pauli on 9/11/2022.
//

#ifndef CANASTA_CPP_UI_ENUMS_H
#define CANASTA_CPP_UI_ENUMS_H

namespace Canasta {
    enum Suit {
        CLUBS,
        HEARTS,
        SPADES,
        DIAMONDS
    };
    enum PreCommands {
        SAVE,
        TAKE,
        HELP,
        QUIT
    };
    enum DrawCommands {
        STOCK,
        DISCARD,
        DRAW_HELP
    };
    enum MeldCommands {
        CREATE,
        ADD,
        MELD_HELP,
        NONE
    };
}

#endif //CANASTA_CPP_UI_ENUMS_H

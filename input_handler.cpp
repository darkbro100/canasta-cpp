//
// Created by pauli on 9/11/2022.
//

#include "input_handler.h"

namespace Canasta {
    static int handleInput() {
        int code;
        std::cin >> code;

        // if we fail to find something, set code to -1
        if (std::cin.fail()) {
            code = -1;
            std::cin.clear();
            std::cin.ignore();
        }

        return code;
    }

    int selectMeld(std::vector<Meld *> &melds, const char *initMsg) {
        return 0;
    }

    int selectCard(Deck *deck, const char *initMsg) {
        std::cout << initMsg << std::endl;

        int count = 0;
        for (auto &c: *deck) {
            std::cout << "[" << (count + 1) << "] " << c << " ";
            count++;
        }
        std::cout << std::endl;

        int select = handleInput() - 1;
        if (select < 0 || select >= count) {
            return selectCard(deck, initMsg);
        }

        return select;
    }

    PreCommands displayPreRound() {
        for (auto &message: preRoundMessages)
            printf("%s", message);

        int code = handleInput();
        return static_cast<Canasta::PreCommands>(code - 1);
    }

    MeldCommands displayMeldOptions(bool addNew, bool addExist) {
        if(!addNew && !addExist) {
            std::cout << "Nothing can be done to current melds." << std::endl;
            return MeldCommands::NONE;
        }

        std::cout << meldMessages[0];

        for(int i = 1; i < 4; i++) {
            if(i == 1 && !addNew)
                continue;

            if(i == 2 && !addExist)
                continue;

            std::cout << "[" << i << "] " << meldMessages[i];
        }

        int code = handleInput();
        return static_cast<Canasta::MeldCommands>(code - 1);
    }

    DrawCommands displayDrawOptions() {
        for (auto &message: drawMessages)
            printf("%s", message);

        int code = handleInput();
        return static_cast<Canasta::DrawCommands>(code - 1);
    }
}
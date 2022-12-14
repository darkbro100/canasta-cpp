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
        std::cout << initMsg << std::endl;

        int count = 0;
        for (auto &m: melds) {
            if (count == 0) {
                count++;
                continue;
            }

            std::cout << "[" << count << "]: " << *m << "\t";
            count++;
        }
        std::cout << std::endl;

        int select = handleInput();
        if (select < 1 || select >= count)
            return selectMeld(melds, initMsg);

        return select;
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
        std::cout << meldMessages[0];

        int length = sizeof(meldMessages) / sizeof(char*);
        for (int i = 1; i < length; i++) {
            if (i == 1 && !addNew) //reference meldMessages to find out what is at index 1 & 2
                continue;

            if (i == 2 && !addExist) //reference meldMessages to find out what is at index 1 & 2
                continue;

            std::cout << "[" << i << "] " << meldMessages[i];
        }

        int code = handleInput() - 1; //subtract one so we can easily convert to the enum

        if(code == MeldCommands::CREATE && !addNew) {
            std::cout << "Unknown command..." << std::endl;
            return displayMeldOptions(addNew, addExist);
        } else if(code == MeldCommands::ADD && !addExist) {
            std::cout << "Unknown command..." << std::endl;
            return displayMeldOptions(addNew, addExist);
        }

        return static_cast<Canasta::MeldCommands>(code);
    }

    DrawCommands displayDrawOptions() {
        for (auto &message: drawMessages)
            printf("%s", message);

        int code = handleInput();
        return static_cast<Canasta::DrawCommands>(code - 1);
    }

    int createMeldWith(int maxAllowed) {
        std::cout << "Input how many cards you want to create the meld with (numbers allowed: [" << MELD_COUNT << " - " << maxAllowed << "] )" << std::endl;
        int selected = handleInput();

        if (selected < MELD_COUNT || selected > maxAllowed) {
            std::cout << "Invalid amount of cards selected. Using " << MELD_COUNT << " cards to start with."
                      << std::endl;
            selected = MELD_COUNT;
        }

        return selected;
    }

    int displayShouldGoOut() {
        std::cout << "You can go out this turn! If you want to, type 1. Type anything else to continue playing." << std::endl;\
        return handleInput();
    }

    int displayCoinToss() {
        std::cout << "Heads or Tails? [0] for heads, [1] for tails." << std::endl;
        int i = handleInput();

        while(i < 0 || i > 1) {
            std::cout << "Invalid input." << std::endl;
            std::cout << "Heads or Tails? [0] for heads, [1] for tails." << std::endl;
            i = handleInput();
        }

        return i;
    }

    int displayRoundRestart() {
        std::cout << "Would you like to reset and play another round? [1] to restart, [0] to stop playing." << std::endl;
        int i = handleInput();

        if(i < 0 || i > 1)
            return displayRoundRestart();

        return i;
    }
}
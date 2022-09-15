//
// Created by pauli on 9/11/2022.
//

#ifndef CANASTA_CPP_INPUT_HANDLER_H
#define CANASTA_CPP_INPUT_HANDLER_H


#include "ui_enums.h"
#include "meld.h"
#include <iostream>

namespace Canasta {
    /**
     * Array containing all the messages that are sent before a round/turn begins
     */
    static const char *preRoundMessages[4] = {
            "[1] Save the game\n",
            "[2] Take turn\n",
            "[3] Ask for help (only before human player plays)\n",
            "[4] Quit the game\n"
    };
    /**
     * An array containing the messages that are sent when asking a human player to interact with melds
     */
    static const char *meldMessages[4] = {
            "What would you like to do with melds?\n",
            "Create new meld\n",
            "Add to existing meld\n",
            "Do nothing\n"
    };

    /**
     * An array containing the messages that are sent when asking the human player how they want to draw
     */
    static const char *drawMessages[3] = {
            "How would you like to draw?\n",
            "[1] Draw from stock pile\n",
            "[2] Take from discard pile\n"
    };

    /**
      * Displays all pre round information specified at
      * @see https://pages.ramapo.edu/~amruth/teaching/opl/projects/canasta/problem.html
      * @return The code entered after pre round input was gathered
      */
    Canasta::PreCommands displayPreRound();

    /**
     * Displays the meld options to the player
     * @param addNew Whether or not the player can create a new meld
     * @param addExist Whether or not the player can add to an existing meld
     * @return The meld command the player chose
     */
    Canasta::MeldCommands displayMeldOptions(bool addNew = true, bool addExist = true);

    /**
     * Displays the draw options to the player
     * @return The draw command that the player used
     */
    Canasta::DrawCommands displayDrawOptions();

    /**
     * Displays to the player and asks them if they want to go out.
     * @return The input given by the player
     */
    int displayShouldGoOut();

    /**
     * Prompts the user to select a card from a given deck. If the player selects an invalid card, the function will recursively call itself and repeat the process until the user selects the correct card.
     * @param deck Pointer to a deck
     * @param initMsg Initial message to send before sending the deck
     * @return The index selected
     */
    int selectCard(Deck *deck, const char *initMsg);

    /**
      * Prompts the user to select a meld from a given set of melds. If the player selects an invalid meld, the function will recursively call itself and repeat the process until the user selects the correct meld.
      * @param melds reference to the vector containing all the melds
      * @param initMsg Initial message to send before sending the melds
      * @return The index selected
      */
    int selectMeld(std::vector<Meld *> &melds, const char *initMsg);

    /**
     * Will ask the user how many cards they want to initially create their meld with.
     * @param maxAllowed Max number of cards they can meld with
     * @return Amount of cards chosen for meld
     */
    int createMeldWith(int maxAllowed);
}

#endif //CANASTA_CPP_INPUT_HANDLER_H

#include <iostream>
#include <Windows.h>
#include <vector>
#include <tuple>
#include "Deck.h"


int BlackJackRound(){

    int deckAmount = 1;
    std::pair<int, std::string> currentCard;

    std::cout << "\nPlease enter the amount of decks should be in the stack: ";
    std::cin >> deckAmount;

    Deck deck(deckAmount);

    //deck.print_deck();

    std::cout << "\nShuffling the deck.\n";

    deck.shuffle_deck();

    //deck.print_deck();

    std::cout << "\nDrawing a Card.\n";

    currentCard = deck.drawCard();

    std::cout << currentCard.second;

    return 0;

}

int main()
{
    int menu_choice = 0;
    bool menu_picked = false;

    while (!menu_picked){

        system("cls");

        std::cout << "BlackJack Basic Training Simulator\n";

        std::cout << "Menu\n\n1. Play round of Black Jack\n2. Run Simulations to Generate Strategy Chart\n3. Quit";

        std::cout << "\nMenu Choice: ";

        std::cin >> menu_choice;

        switch (menu_choice) {

            default:
                std::cout << "\nPlease select on of the options in the menu!\n";
                Sleep(1000);
                break;

            case 1:
                std::cout << "\nStarting Round of Black Jack\n";
                menu_picked = true;
                BlackJackRound();
                break;

            case 2:
                std::cout << "\nStarting Simulation\n";
                menu_picked = true;
                break;

            case 3:
                std::cout << "\nQuitting Program\n";
                menu_picked = true;
                return 0;
        }

    }

}

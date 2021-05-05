#include <iostream>
#include <Windows.h>
#include <shellapi.h>
#include <vector>
#include <tuple>
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"


int BlackJackRound(){

    int deckAmount = 1;
    std::pair<int, std::string> currentCard;
    std::string playerName, choice;
    bool game_over = false, chosen = false, player_stand = false, card_revealed = false, p_blackjack = false, p_bust = false, d_blackjack = false, d_bust = false;

    std::cout << "\nPlease enter the Players Name: ";
    std::cin >> playerName;

    Player player(playerName);
    Dealer dealer;

    std::cout << "\nPlease enter the amount of decks should be in the stack: ";
    std::cin >> deckAmount;

    Deck deck(deckAmount);

    //deck.print_deck();

    std::cout << "\nShuffling the deck.\n";

    deck.shuffle_deck();

    //deck.print_deck();

    std::cout << "\nDistributing initial set.\n";

    currentCard = deck.drawCard();
    player.add_card(currentCard);
    player.print_hand();

    currentCard = deck.drawCard();
    dealer.add_card(currentCard);
    dealer.print_hand();

    currentCard = deck.drawCard();
    player.add_card(currentCard);
    player.print_hand();

    currentCard = deck.drawCard();
    dealer.add_card(currentCard);
    dealer.print_half_hand();

    if (player.get_value() == 21 && dealer.get_value() == 21){

        //player and dealer has blackjack
//        std::cout << "\nplayer and dealer has blackjack\n";

        p_blackjack = true;
        d_blackjack = true;

        game_over = true;

    }
    else if (player.get_value() == 21 && dealer.get_value() != 21){

        //player got blackjack
//        std::cout << "\nplayer got blackjack\n";

        p_blackjack = true;

        game_over = true;

    }
    else if (dealer.get_value() == 21 && player.get_value() != 21 ){

        //dealer got blackjack
//        std::cout << "\ndealer got blackjack\n";

        d_blackjack = true;

        game_over = true;

    }

    while (!game_over){

        while (!chosen && !player_stand) {

            std::cout << "\nDo you want to Hit (H) or Stand (S): ";
            std::cin >> choice;

            if (choice == "hit" || choice == "h" || choice == "Hit" || choice == "H"){

                //player has hit

                currentCard = deck.drawCard();
                player.add_card(currentCard);
                player.print_hand();

                chosen = true;

            }
            else if (choice == "stand" || choice == "s" || choice == "Stand" || choice == "S"){

                //player has chosen stand

                player_stand = true;
                chosen = true;

            }
            else{

                std::cout << "\nPlease either choose hit or stand by typing either H or S\n";

            }
        }

        chosen = false;

        if (player.get_value() == 21 && dealer.get_value() == 21){

           //player and dealer has blackjack
//            std::cout << "\nplayer and dealer has blackjack\n";

            p_blackjack = true;
            d_blackjack = true;

            game_over = true;
        }
        else if (player.get_value() == 21 && dealer.get_value() != 21){

           //player got blackjack
//            std::cout << "\nplayer got blackjack\n";

            p_blackjack = true;

            game_over = true;

        }
        else if (dealer.get_value() == 21 && player.get_value() != 21 ){

           //dealer got blackjack
//            std::cout << "\ndealer got blackjack\n";

            d_blackjack = true;

            game_over = true;

        }
        else if (player.get_value() > 21){

           //player has bust
//            std::cout << "\nplayer has bust\n";

            p_bust = true;

            game_over = true;

        }
        else if (dealer.get_value() > 21){

            //dealer has bust
            //std::cout << "\ndealer has bust\n";

            d_bust = true;

            game_over = true;

        }
        else{

           //nothing special happened keep playing
            //std::cout << "\nothing special happened keep playing\n";

        }

        if (player_stand){

            //dealer will reveal his card and keep drawing until either bust or standing at soft 17

            if (!card_revealed){

                std::cout << "\nThe dealer reveals his face down card!\n";
                dealer.print_hand();
                card_revealed = true;

            }


            if (dealer.get_soft_value() >= 17){

                //dealer will stand on soft 17 or higher
                //std::cout << "\nThe dealer has soft 17 or higher and will stand\n";

                game_over = true;

            }
            else{

                //dealer will keep drawing
                currentCard = deck.drawCard();
                dealer.add_card(currentCard);
                dealer.print_hand();


            }

        }

    }

    std::cout << "\nThe round is over and the final hands are as follows:\n";

    player.print_hand();
    dealer.print_hand();

    if (p_blackjack && d_blackjack){

        //player and dealer both have blackjacks and therefore have pushed

        std::cout << "\nBoth the player and the dealer have hit the blackjack, game is a push and player gets his chips back!\n";

        return 3;

    }
    else if (p_blackjack){

        //player has blackjack and takes 1.5 times their bet

        std::cout << "\nThe player has hit the blackjack, they get 1.5x of what they bet in addition of original chips!\n";

        return 1;

    }
    else if (d_blackjack){

        //dealer has blackjack and player looses their bet

        std::cout << "\nThe dealer has hit the blackjack, the player looses their bet!\n";

        return 2;

    }
    else if (p_bust){

        //player has gone bust and looses their bet

        std::cout << "\nThe player has busted, they loose their bet!\n";

        return 2;

    }
    else if (d_bust){

        //dealer has gone bust and player gets back 1x their chips and their original bet

        std::cout << "\nThe dealer has busted, the player gets 1x of what they bet in addition of original chips!\n";

        return 1;

    }
    else if (player.get_value() == dealer.get_value()){

        //player and dealer have pushed on the same number, the player gets their chips back

        std::cout << "\nBoth the player and the dealer have the same number, game is a push and player gets his chips back!\n";

        return 3;

    }
    else if (player.get_value() > dealer.get_value()){

        //player has more score than dealer and get's back 1x their chips and their original bet

        std::cout << "\nThe player has a higher score than the dealer, the player gets 1x of what they bet in addition of original chips!\n";

        return 1;

    }
    else if (dealer.get_value() > player.get_value()){

        //dealer has more score than player and player looses their bet

        std::cout << "\nThe dealer has a higher score than the player,  the player looses their bet!\n";

        return 2;

    }
    else{

        //This should not happen but here is an easter egg.
        std::cout << "\nOh no something has gone terribly wrong\n";
        ShellExecute(nullptr, "open", "https://www.youtube.com/watch?v=dQw4w9WgXcQ",nullptr, nullptr, SW_SHOWNORMAL);

        return 0;

    }

}

int BlackJackSim(int deckAmount){

    std::pair<int, std::string> currentCard;
    std::string playerName, choice;
    bool game_over = false, chosen = false, player_stand = false, card_revealed = false, p_blackjack = false, p_bust = false, d_blackjack = false, d_bust = false;

    Player player("ree");
    Dealer dealer;

    Deck deck(deckAmount);

    //deck.print_deck();

    std::cout << "\nShuffling the deck.\n";

    deck.shuffle_deck();

    //deck.print_deck();

    std::cout << "\nDistributing initial set.\n";

    currentCard = deck.drawCard();
    player.add_card(currentCard);
    player.print_hand();

    currentCard = deck.drawCard();
    dealer.add_card(currentCard);
    dealer.print_hand();

    currentCard = deck.drawCard();
    player.add_card(currentCard);
    player.print_hand();

    currentCard = deck.drawCard();
    dealer.add_card(currentCard);
    dealer.print_half_hand();

    if (player.get_value() == 21 && dealer.get_value() == 21){

        //player and dealer has blackjack
//        std::cout << "\nplayer and dealer has blackjack\n";

        p_blackjack = true;
        d_blackjack = true;

        game_over = true;

    }
    else if (player.get_value() == 21 && dealer.get_value() != 21){

        //player got blackjack
//        std::cout << "\nplayer got blackjack\n";

        p_blackjack = true;

        game_over = true;

    }
    else if (dealer.get_value() == 21 && player.get_value() != 21 ){

        //dealer got blackjack
//        std::cout << "\ndealer got blackjack\n";

        d_blackjack = true;

        game_over = true;

    }

    while (!game_over){

        while (!chosen && !player_stand) {

            std::cout << "\nDo you want to Hit (H) or Stand (S): ";
            std::cin >> choice;

            if (choice == "hit" || choice == "h" || choice == "Hit" || choice == "H"){

                //player has hit

                currentCard = deck.drawCard();
                player.add_card(currentCard);
                player.print_hand();

                chosen = true;

            }
            else if (choice == "stand" || choice == "s" || choice == "Stand" || choice == "S"){

                //player has chosen stand

                player_stand = true;
                chosen = true;

            }
            else{

                std::cout << "\nPlease either choose hit or stand by typing either H or S\n";

            }
        }

        chosen = false;

        if (player.get_value() == 21 && dealer.get_value() == 21){

            //player and dealer has blackjack
//            std::cout << "\nplayer and dealer has blackjack\n";

            p_blackjack = true;
            d_blackjack = true;

            game_over = true;
        }
        else if (player.get_value() == 21 && dealer.get_value() != 21){

            //player got blackjack
//            std::cout << "\nplayer got blackjack\n";

            p_blackjack = true;

            game_over = true;

        }
        else if (dealer.get_value() == 21 && player.get_value() != 21 ){

            //dealer got blackjack
//            std::cout << "\ndealer got blackjack\n";

            d_blackjack = true;

            game_over = true;

        }
        else if (player.get_value() > 21){

            //player has bust
//            std::cout << "\nplayer has bust\n";

            p_bust = true;

            game_over = true;

        }
        else if (dealer.get_value() > 21){

            //dealer has bust
            //std::cout << "\ndealer has bust\n";

            d_bust = true;

            game_over = true;

        }
        else{

            //nothing special happened keep playing
            //std::cout << "\nothing special happened keep playing\n";

        }

        if (player_stand){

            //dealer will reveal his card and keep drawing until either bust or standing at soft 17

            if (!card_revealed){

                std::cout << "\nThe dealer reveals his face down card!\n";
                dealer.print_hand();
                card_revealed = true;

            }


            if (dealer.get_soft_value() >= 17){

                //dealer will stand on soft 17 or higher
                //std::cout << "\nThe dealer has soft 17 or higher and will stand\n";

                game_over = true;

            }
            else{

                //dealer will keep drawing
                currentCard = deck.drawCard();
                dealer.add_card(currentCard);
                dealer.print_hand();


            }

        }

    }

    std::cout << "\nThe round is over and the final hands are as follows:\n";

    player.print_hand();
    dealer.print_hand();

    if (p_blackjack && d_blackjack){

        //player and dealer both have blackjacks and therefore have pushed

        std::cout << "\nBoth the player and the dealer have hit the blackjack, game is a push and player gets his chips back!\n";

        return 3;

    }
    else if (p_blackjack){

        //player has blackjack and takes 1.5 times their bet

        std::cout << "\nThe player has hit the blackjack, they get 1.5x of what they bet in addition of original chips!\n";

        return 1;

    }
    else if (d_blackjack){

        //dealer has blackjack and player looses their bet

        std::cout << "\nThe dealer has hit the blackjack, the player looses their bet!\n";

        return 2;

    }
    else if (p_bust){

        //player has gone bust and looses their bet

        std::cout << "\nThe player has busted, they loose their bet!\n";

        return 2;

    }
    else if (d_bust){

        //dealer has gone bust and player gets back 1x their chips and their original bet

        std::cout << "\nThe dealer has busted, the player gets 1x of what they bet in addition of original chips!\n";

        return 1;

    }
    else if (player.get_value() == dealer.get_value()){

        //player and dealer have pushed on the same number, the player gets their chips back

        std::cout << "\nBoth the player and the dealer have the same number, game is a push and player gets his chips back!\n";

        return 3;

    }
    else if (player.get_value() > dealer.get_value()){

        //player has more score than dealer and get's back 1x their chips and their original bet

        std::cout << "\nThe player has a higher score than the dealer, the player gets 1x of what they bet in addition of original chips!\n";

        return 1;

    }
    else if (dealer.get_value() > player.get_value()){

        //dealer has more score than player and player looses their bet

        std::cout << "\nThe dealer has a higher score than the player,  the player looses their bet!\n";

        return 2;

    }
    else{

        //This should not happen but here is an easter egg.
        std::cout << "\nOh no something has gone terribly wrong\n";
        ShellExecute(nullptr, "open", "https://www.youtube.com/watch?v=dQw4w9WgXcQ",nullptr, nullptr, SW_SHOWNORMAL);

        return 0;

    }

}

int simulation(){

    int deckAmount = 0, sim_a = 0;

    std::cout << "\nHow many sims you want to run: ";
    std::cin >> deckAmount;

    std::cout << "\nPlease enter the amount of decks should be in the stack: ";
    std::cin >> deckAmount;

    //TODO Finish sim section tomorow

    BlackJackSim(deckAmount);

    return 0;

}

int main()
{
    int menu_choice = 0, game_result;
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
                game_result = BlackJackRound();
                break;

            case 2:
                std::cout << "\nStarting Simulation\n";
                simulation();
                break;

            case 3:
                std::cout << "\nQuitting Program\n";
                menu_picked = true;
                return 0;
        }

    }

}

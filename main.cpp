#include <iostream>
#include <Windows.h>
#include <shellapi.h>
#include <vector>
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"
#include <thread>
#include <mutex>
#include <future>
#include <iostream>
#include <fstream>
#include <chrono>
#include "global.h"
#include "farm.h"
#include "sim_task.h"
#include "task.h"

std::pair ace = std::make_pair(1,"ACE");
std::pair two = std::make_pair(2,"TWO");
std::pair three = std::make_pair(3,"THREE");
std::pair four = std::make_pair(4,"FOUR");
std::pair five = std::make_pair(5,"FIVE");
std::pair six = std::make_pair(6,"SIX");
std::pair seven = std::make_pair(7,"SEVEN");
std::pair eight = std::make_pair(8,"EIGHT");
std::pair nine = std::make_pair(9,"NINE");
std::pair ten = std::make_pair(10,"TEN");
std::pair jack = std::make_pair(10,"JACK");
std::pair queen = std::make_pair(10,"QUEEN");
std::pair king = std::make_pair(10,"KING");

std::vector<std::pair<std::pair<int, std::string>,std::pair<int, std::string>>> player_cards;
std::vector<std::pair<int, std::string>> dealer_cards;



std::vector<std::vector<int>> results = {};
std::mutex result_mutex;

std::condition_variable ready_cv;
std::mutex ready_mutex;
bool ready = false;
int deckAmount = 0;

typedef std::chrono::steady_clock the_clock;

the_clock::time_point start = the_clock::now();

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
//        std::cout << "\n player and dealer has blackjack\n";

        p_blackjack = true;
        d_blackjack = true;

        game_over = true;

    }
    else if (player.get_value() == 21 && dealer.get_value() != 21){

        //player got blackjack
//        std::cout << "\n player got blackjack\n";

        p_blackjack = true;

        game_over = true;

    }
    else if (dealer.get_value() == 21 && player.get_value() != 21 ){

        //dealer got blackjack
//        std::cout << "\n dealer got blackjack\n";

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
//            std::cout << "\n player and dealer has blackjack\n";

            p_blackjack = true;
            d_blackjack = true;

            game_over = true;
        }
        else if (player.get_value() == 21 && dealer.get_value() != 21){

            //player got blackjack
//            std::cout << "\n player got blackjack\n";

            p_blackjack = true;

            game_over = true;

        }
        else if (dealer.get_value() == 21 && player.get_value() != 21 ){

            //dealer got blackjack
//            std::cout << "\n dealer got blackjack\n";

            d_blackjack = true;

            game_over = true;

        }
        else if (player.get_value() > 21){

            //player has bust
//            std::cout << "\n player has bust\n";

            p_bust = true;

            game_over = true;

        }
        else if (dealer.get_value() > 21){

            //dealer has bust
            //std::cout << "\n dealer has bust\n";

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

    Sleep(2000);

    if (p_blackjack && d_blackjack){

        //player and dealer both have blackjacks and therefore have pushed

        std::cout << "\nBoth the player and the dealer have hit the blackjack, game is a push and player gets his chips back!\n";

        Sleep(2000);

        return 3;

    }
    else if (p_blackjack){

        //player has blackjack and takes 1.5 times their bet

        std::cout << "\nThe player has hit the blackjack, they get 1.5x of what they bet in addition of original chips!\n";

        Sleep(2000);

        return 1;

    }
    else if (d_blackjack){

        //dealer has blackjack and player looses their bet

        std::cout << "\nThe dealer has hit the blackjack, the player looses their bet!\n";

        Sleep(2000);

        return 2;

    }
    else if (p_bust){

        //player has gone bust and looses their bet

        std::cout << "\nThe player has busted, they loose their bet!\n";

        Sleep(2000);

        return 2;

    }
    else if (d_bust){

        //dealer has gone bust and player gets back 1x their chips and their original bet

        std::cout << "\nThe dealer has busted, the player gets 1x of what they bet in addition of original chips!\n";

        Sleep(2000);

        return 1;

    }
    else if (player.get_value() == dealer.get_value()){

        //player and dealer have pushed on the same number, the player gets their chips back

        std::cout << "\nBoth the player and the dealer have the same number, game is a push and player gets his chips back!\n";

        Sleep(2000);

        return 3;

    }
    else if (player.get_value() > dealer.get_value()){

        //player has more score than dealer and gets back 1x their chips and their original bet

        std::cout << "\nThe player has a higher score than the dealer, the player gets 1x of what they bet in addition of original chips!\n";

        Sleep(2000);

        return 1;

    }
    else if (dealer.get_value() > player.get_value()){

        //dealer has more score than player and player looses their bet

        std::cout << "\nThe dealer has a higher score than the player,  the player looses their bet!\n";

        Sleep(2000);

        return 2;

    }
    else{

        //This should not happen but here is an easter egg.
        std::cout << "\nOh no something has gone terribly wrong\n";
        ShellExecute(nullptr, "open", "https://www.youtube.com/watch?v=dQw4w9WgXcQ",nullptr, nullptr, SW_SHOWNORMAL);

        Sleep(2000);

        return 0;

    }

}

int simulation_nonpar() {

    int deckAmount = 0, sim_a = 0;

    results.clear();

    std::cout << "\nHow many sims you want to run: ";
    std::cin >> sim_a;

    std::cout << "\nPlease enter the amount of decks should be in the stack: ";
    std::cin >> deckAmount;

//    farm.add_task(new SimTask(deckAmount, player_cards[0], dealer_cards[0], "h"));

    std::cout << "\nRunning non parallel sim now, please stand by!\n";
    the_clock::time_point start = the_clock::now();

    for (int s = 0; s < sim_a; s++){
        
        SimTaskNonPar(deckAmount, std::make_pair(five, five), ace, "h").run();
        SimTaskNonPar(deckAmount, std::make_pair(five, five), two, "h").run();
        SimTaskNonPar(deckAmount, std::make_pair(five, five), three, "h").run();
        SimTaskNonPar(deckAmount, std::make_pair(five, five), four, "h").run();
        SimTaskNonPar(deckAmount, std::make_pair(five, five), five, "h").run();
        SimTaskNonPar(deckAmount, std::make_pair(five, five), six, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, five), seven, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, five), eight, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, five), nine, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, five), ten, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, five), jack, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, five), queen, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, five), king, "h").run();

       SimTaskNonPar(deckAmount, std::make_pair(five, six), ace, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, six), two, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, six), three, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, six), four, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, six), five, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, six), six, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, six), seven, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, six), eight, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, six), nine, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, six), ten, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, six), jack, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, six), queen, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(five, six), king, "h").run();

       SimTaskNonPar(deckAmount, std::make_pair(six, six), ace, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, six), two, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, six), three, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, six), four, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, six), five, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, six), six, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, six), seven, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, six), eight, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, six), nine, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, six), ten, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, six), jack, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, six), queen, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, six), king, "h").run();

       SimTaskNonPar(deckAmount, std::make_pair(six, seven), ace, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, seven), two, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, seven), three, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, seven), four, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, seven), five, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, seven), six, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, seven), seven, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, seven), eight, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, seven), nine, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, seven), ten, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, seven), jack, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, seven), queen, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(six, seven), king, "h").run();

       SimTaskNonPar(deckAmount, std::make_pair(seven, seven), ace, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, seven), two, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, seven), three, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, seven), four, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, seven), five, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, seven), six, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, seven), seven, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, seven), eight, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, seven), nine, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, seven), ten, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, seven), jack, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, seven), queen, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, seven), king, "h").run();

       SimTaskNonPar(deckAmount, std::make_pair(seven, eight), ace, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, eight), two, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, eight), three, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, eight), four, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, eight), five, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, eight), six, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, eight), seven, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, eight), eight, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, eight), nine, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, eight), ten, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, eight), jack, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, eight), queen, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(seven, eight), king, "h").run();

       SimTaskNonPar(deckAmount, std::make_pair(eight, eight), ace, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, eight), two, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, eight), three, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, eight), four, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, eight), five, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, eight), six, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, eight), seven, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, eight), eight, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, eight), nine, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, eight), ten, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, eight), jack, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, eight), queen, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, eight), king, "h").run();

       SimTaskNonPar(deckAmount, std::make_pair(eight, nine), ace, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, nine), two, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, nine), three, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, nine), four, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, nine), five, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, nine), six, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, nine), seven, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, nine), eight, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, nine), nine, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, nine), ten, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, nine), jack, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, nine), queen, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(eight, nine), king, "h").run();

       SimTaskNonPar(deckAmount, std::make_pair(nine, nine), ace, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(nine, nine), two, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(nine, nine), three, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(nine, nine), four, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(nine, nine), five, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(nine, nine), six, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(nine, nine), seven, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(nine, nine), eight, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(nine, nine), nine, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(nine, nine), ten, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(nine, nine), jack, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(nine, nine), queen, "h").run();
       SimTaskNonPar(deckAmount, std::make_pair(nine, nine), king, "h").run();

    }

    the_clock::time_point end = the_clock::now();

    auto time_taken = duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "\nRunning " << sim_a << " simulations in non parallel (total of " << sim_a*117 << " scenarios) took " << time_taken << " ms!\n";

//    for (auto i : results){
//
//        for (auto j : i){
//
//            std::cout << j << " ";
//
//        }
//
//        std::cout << "\n";
//
//    }

    std::ofstream outfile;
    outfile.open("outfile.csv");

    for (const auto& i : results) {

        for (auto j : i) {

//            std::cout << j << " ";
            outfile << j << ",";

        }

        outfile << ",\n";

    }

    outfile.close();

    Sleep(5000);

    system("pause");

    return 0;

}

int simulation() {

    int sim_a = 0;
    bool valid = false;

    while(!valid){

        std::cout << "\nHow many sims you want to run: ";
        std::cin >> sim_a;

        if (sim_a > 68100){

            std::cout << "\nSorry you can only run a max of 68,100 simulations!\n";
            Sleep(2000);

        }
        else{

            valid = true;

        }

    }

    Farm farm;
    farm.add_task(new StartTask());

    results.clear();

//    farm.add_task(new SimTask(deckAmount, player_cards[0], dealer_cards[0], "h"));
    int tmp = 0;
    for (int s = 0; s < sim_a; s++){

        for (int i = 0; i < 9; i++){

            //std::cout << "\nPlayer" << i << "\n";

            for (int j = 0; j < 13; j++){

                farm.add_task(new SimTask(player_cards[i], dealer_cards[j], "h", tmp));
//                if (tmp % 1000 == 0) std::cout << tmp << "\n";
//                //std::cout << "\nDealer" << j << "\n";
                tmp++;

            }

        }

    }

    std::cout << "\nRunning " << farm.size() << " threads now, please stand by!\n";


    farm.run();

    the_clock::time_point end = the_clock::now();

    auto time_taken = duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "\nRunning " << sim_a << " simulations (total of " << sim_a*117 << " scenarios) took " << time_taken << " ms!\n";

//    for (auto i : results){
//
//        for (auto j : i){
//
//            std::cout << j << " ";
//
//        }
//
//        std::cout << "\n";
//
//    }

    std::ofstream outfile;
    outfile.open("outfile.csv");

    for (const auto& i : results) {

        for (auto j : i) {

//            std::cout << j << " ";
            outfile << j << ",";

        }

        outfile << ",\n";

    }

    outfile.close();

    Sleep(5000);

    system("pause");

    return 0;

}


int main()
{
    int menu_choice = 0, game_result;
    bool menu_picked = false;

    player_cards.emplace_back(std::make_pair(five,five));
    player_cards.emplace_back(std::make_pair(five,six));
    player_cards.emplace_back(std::make_pair(six,six));
    player_cards.emplace_back(std::make_pair(six,seven));
    player_cards.emplace_back(std::make_pair(seven,seven));
    player_cards.emplace_back(std::make_pair(seven,eight));
    player_cards.emplace_back(std::make_pair(eight,eight));
    player_cards.emplace_back(std::make_pair(eight,nine));
    player_cards.emplace_back(std::make_pair(nine,nine));

    dealer_cards.emplace_back(ace);
    dealer_cards.emplace_back(two);
    dealer_cards.emplace_back(three);
    dealer_cards.emplace_back(four);
    dealer_cards.emplace_back(five);
    dealer_cards.emplace_back(six);
    dealer_cards.emplace_back(seven);
    dealer_cards.emplace_back(eight);
    dealer_cards.emplace_back(nine);
    dealer_cards.emplace_back(ten);
    dealer_cards.emplace_back(jack);
    dealer_cards.emplace_back(queen);
    dealer_cards.emplace_back(king);





    while (!menu_picked){

        system("cls");

        std::cout << "BlackJack Basic Training Simulator\n";

        std::cout << "Menu\n\n1. Play round of Black Jack\n2. Run Simulations to Generate Strategy Chart\n3. Run Simulations to Generate Strategy Chart (SingleThread Mode)\n4. Quit";

        std::cout << "\nMenu Choice: ";

        std::cin >> menu_choice;

        switch (menu_choice) {

            default:
                std::cout << "\nPlease select on of the options in the menu!\n";
                Sleep(1000);
                break;

            case 1:
                std::cout << "\nStarting Round of Black Jack\n";
                BlackJackRound();
                break;

            case 2:
                std::cout << "\nStarting Simulation\n";
                simulation();
                break;

            case 3:
                std::cout << "\nStarting Non Par Simulation\n";
                simulation_nonpar();
                break;

            case 4:
                std::cout << "\nQuitting Program\n";
                menu_picked = true;
                return 0;
        }

    }

}

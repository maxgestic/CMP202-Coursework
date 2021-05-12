#include "sim_task.h"
#include "vector"
#include "Player.h"
#include "Dealer.h"
#include "Deck.h"
#include <Windows.h>
#include <shellapi.h>
#include <iostream>
#include "global.h"
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <thread>

void SimTask::run() {

    if (count_ < 100){ //check to make sure the while loop and mutex dont run for every task as this slows down the program massivley

        std::unique_lock<std::mutex> lock(ready_mutex);

        while(!ready){

            ready_cv.wait(lock);

        }

    }

    std::vector<int> return_vector;

    std::pair<int, std::string> currentCard;
    std::string playerName;
    bool game_over = false, chosen = false, player_stand = false, card_revealed = false, p_blackjack = false, p_bust = false, d_blackjack = false, d_bust = false;

    Player player("sim");
    Dealer dealer;

    Deck deck(deckAmount);

    //deck.print_deck();

    deck.shuffle_deck();

    //deck.print_deck();

    player.add_card(player_hand_.first);
    player.add_card(player_hand_.second);

    dealer.add_card(dealer_hand_);

    return_vector.emplace_back(player.get_value());
    return_vector.emplace_back(dealer.get_value());

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

            if (choice_ == "h"){

                //player has hit

                currentCard = deck.drawCard();
                player.add_card(currentCard);

                chosen = true;

            }
            else if (choice_ == "s"){

                //player has chosen stand

                player_stand = true;
                chosen = true;

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


            }

        }

    }

    if (p_blackjack && d_blackjack){

        //player and dealer both have blackjacks and therefore have pushed

        return_vector.emplace_back(3);

    }
    else if (p_blackjack){

        //player has blackjack and takes 1.5 times their bet

        return_vector.emplace_back(1);

    }
    else if (d_blackjack){

        //dealer has blackjack and player looses their bet

        return_vector.emplace_back(2);

    }
    else if (p_bust){

        //player has gone bust and looses their bet

        return_vector.emplace_back(2);

    }
    else if (d_bust){

        //dealer has gone bust and player gets back 1x their chips and their original bet

        return_vector.emplace_back(1);

    }
    else if (player.get_value() == dealer.get_value()){

        //player and dealer have pushed on the same number, the player gets their chips back

        return_vector.emplace_back(3);

    }
    else if (player.get_value() > dealer.get_value()){

        //player has more score than dealer and get's back 1x their chips and their original bet

        return_vector.emplace_back(1);

    }
    else if (dealer.get_value() > player.get_value()){

        //dealer has more score than player and player looses their bet

        return_vector.emplace_back(2);

    }
    else{

        //This should not happen but here is an easter egg.
        ShellExecute(nullptr, "open", "https://www.youtube.com/watch?v=dQw4w9WgXcQ",nullptr, nullptr, SW_SHOWNORMAL);


        return_vector.emplace_back(0);

    }

    result_mutex.lock();
    results.emplace_back(return_vector);
    result_mutex.unlock();
}

void SimTaskNonPar::run() {

    std::vector<int> return_vector;

    std::pair<int, std::string> currentCard;
    std::string playerName;
    bool game_over = false, chosen = false, player_stand = false, card_revealed = false, p_blackjack = false, p_bust = false, d_blackjack = false, d_bust = false;

    Player player("sim");
    Dealer dealer;

    Deck deck(deckAmount_);

    //deck.print_deck();

    deck.shuffle_deck();

    //deck.print_deck();

    player.add_card(player_hand_.first);
    player.add_card(player_hand_.second);

    dealer.add_card(dealer_hand_);

    return_vector.emplace_back(player.get_value());
    return_vector.emplace_back(dealer.get_value());

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

            if (choice_ == "h"){

                //player has hit

                currentCard = deck.drawCard();
                player.add_card(currentCard);

                chosen = true;

            }
            else if (choice_ == "s"){

                //player has chosen stand

                player_stand = true;
                chosen = true;

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


            }

        }

    }

    if (p_blackjack && d_blackjack){

        //player and dealer both have blackjacks and therefore have pushed

        return_vector.emplace_back(3);

    }
    else if (p_blackjack){

        //player has blackjack and takes 1.5 times their bet

        return_vector.emplace_back(1);

    }
    else if (d_blackjack){

        //dealer has blackjack and player looses their bet

        return_vector.emplace_back(2);

    }
    else if (p_bust){

        //player has gone bust and looses their bet

        return_vector.emplace_back(2);

    }
    else if (d_bust){

        //dealer has gone bust and player gets back 1x their chips and their original bet

        return_vector.emplace_back(1);

    }
    else if (player.get_value() == dealer.get_value()){

        //player and dealer have pushed on the same number, the player gets their chips back

        return_vector.emplace_back(3);

    }
    else if (player.get_value() > dealer.get_value()){

        //player has more score than dealer and get's back 1x their chips and their original bet

        return_vector.emplace_back(1);

    }
    else if (dealer.get_value() > player.get_value()){

        //dealer has more score than player and player looses their bet

        return_vector.emplace_back(2);

    }
    else{

        //This should not happen but here is an easter egg.
        ShellExecute(nullptr, "open", "https://www.youtube.com/watch?v=dQw4w9WgXcQ",nullptr, nullptr, SW_SHOWNORMAL);


        return_vector.emplace_back(0);

    }

    result_mutex.lock();
    results.emplace_back(return_vector);
    result_mutex.unlock();
}

void StartTask::run(){

    std::unique_lock<std::mutex> lock(ready_mutex);

    std::cout << "\nPlease enter the amount of decks should be in the stack: ";
    std::cin >> deckAmount;

    ready = true;

    ready_cv.notify_all();

    start = std::chrono::steady_clock::now();

};
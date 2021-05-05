#include "Dealer.h"
#include <utility>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

Dealer::Dealer() {
    
    handValue = 0;

}

void Dealer::add_card(const std::pair<int, std::string>& card) {

    hand.emplace_back(card);

}

int Dealer::get_value() {

    std::vector<int> countVector;
    int sum = 0, temp_sum = 0;

    for(auto & i : hand)
    {
        countVector.emplace_back(i.first);
    }

    for (auto & i : countVector){

        temp_sum += i;

    }

    while (temp_sum < 21){

        if(temp_sum + 11 < 21 && std::count(countVector.begin(), countVector.end(), 1)){

            auto it = std::find(countVector.begin(), countVector.end(), 1);

            if (it != countVector.end()){

                countVector[*it] = 11;

            }

            temp_sum += 10;

        }
        else {

            break;

        }

    }

    sum = temp_sum;



    return sum;

}

void Dealer::print_hand() {

    std::cout << "\nYour current hand is: \n";

    for(auto & i : hand)
    {
        std::cout  << "|" << i.second << "| ";
    }

    std::cout << "\nThe value of your hand is: " << get_value() << "\n";



}


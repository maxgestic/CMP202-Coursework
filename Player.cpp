#include "Player.h"
#include <utility>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

Player::Player(std::string name) {

    playerName = std::move(name);
    handValue = 0;

}

void Player::add_card(const std::pair<int, std::string>& card) {

    hand.emplace_back(card);

}

int Player::get_value() {

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

        if(temp_sum + 10 < 21 && std::count(countVector.begin(), countVector.end(), 1)){

            auto it = std::find(countVector.begin(), countVector.end(), 1);

            if (it != countVector.end()){

                countVector[*it - 1] = 11;

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

void Player::print_hand() {

    std::cout << "\nYour current hand is: \n";

    for(auto & i : hand)
    {
        std::cout  << "|" << i.second << "| ";
    }

    std::cout << "\nThe value of your hand is: " << get_value() << "\n";



}



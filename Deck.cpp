//
// Created by maxri on 05/05/2021.
//

#include "Deck.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>

int ACE = 1, JACK =10, QUEEN =10, KING = 10;

Deck::Deck(int size) {

    //std::cout << "\nCreating pack with " << size << " number of decks\n";

    for (int i = 0; i <= size - 1; i++){

        for (int j = 0; i <= 3; i++){

            deck.emplace_back(ACE,"ACE");
            deck.emplace_back(2,"TWO");
            deck.emplace_back(3,"THREE");
            deck.emplace_back(4,"FOUR");
            deck.emplace_back(5,"FIVE");
            deck.emplace_back(6,"SIX");
            deck.emplace_back(7,"SEVEN");
            deck.emplace_back(8,"EIGHT");
            deck.emplace_back(9,"NINE");
            deck.emplace_back(10,"TEN");
            deck.emplace_back(JACK,"JACK");
            deck.emplace_back(QUEEN,"QUEEN");
            deck.emplace_back(KING,"KING");

        }

    }
}

void Deck::print_deck() {

    std::cout << "Deck size is " << deck.size() << "\nCurrent Deck is: \n";

    for(int i = 0; i < deck.size(); i++)
    {
        std::cout  << "(" << deck[i].first << " | " << deck[i].second << ") ";
    }

}

void Deck::shuffle_deck(){

    auto rd = std::random_device{};
    auto rng = std::default_random_engine {rd()};

    std::shuffle(std::begin(deck), std::end(deck), rng);

}

bool Deck::deckEmpty() {

    return deck.empty();

}

int Deck::deckSize() {

    return deck.size();

}

std::pair<int, std::string> Deck::drawCard() {

    std::pair<int, std::string> frontCard = deck.front();

    deck.erase(deck.begin());

    return frontCard;

}

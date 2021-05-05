#ifndef PROJECT_DECK_H
#define PROJECT_DECK_H
#include <string>
#include <vector>


class Deck {

public:

    explicit Deck(int);
    void print_deck();
    void shuffle_deck();
    bool deckEmpty();
    int deckSize();
    std::pair<int, std::string> drawCard();



private:

    std::vector<std::pair<int, std::string>> deck;

};


#endif //PROJECT_DECK_H

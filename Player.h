#ifndef PROJECT_PLAYER_H
#define PROJECT_PLAYER_H
#include <string>
#include <vector>

class Player {
public:

    Player(std::string);

    void print_hand();
    int get_value();
    void add_card(const std::pair<int, std::string>&);

private:

    std::vector<std::pair<int, std::string>> hand;
    std::string playerName;
    int handValue;

};


#endif //PROJECT_PLAYER_H

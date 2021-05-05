#ifndef PROJECT_DEALER_H
#define PROJECT_DEALER_H
#include <string>
#include <vector>

class Dealer {
public:

    Dealer();

    void print_hand();
    void print_half_hand();
    int get_soft_value();
    int get_value();
    void add_card(const std::pair<int, std::string>&);

private:

    std::vector<std::pair<int, std::string>> hand;
    int handValue;


};


#endif //PROJECT_DEALER_H

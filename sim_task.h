#ifndef SIM_TASK_H
#define SIM_TASK_H

#include <string>
#include <utility>
#include "task.h"
#include <string>
#include <vector>
#include <mutex>
#include "global.h"

/** Task that prints a message to cout. */
class SimTask : public Task
{
public:
    SimTask(const int& deckAmount, std::pair<std::pair<int, std::string>, std::pair<int, std::string>> player_hand, std::pair<int, std::string>  dealer_hand, std::string  choice)
            : deckAmount_(deckAmount), player_hand_(std::move(player_hand)), dealer_hand_(std::move(dealer_hand)), choice_(std::move(choice))
    {
    }

    void run();

private:
    const int deckAmount_;
    const std::pair<std::pair<int, std::string>, std::pair<int, std::string>> player_hand_;
    const std::pair<int, std::string> dealer_hand_;
    const std::string choice_;

};

#endif
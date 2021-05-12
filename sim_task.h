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
    SimTask(std::pair<std::pair<int, std::string>, std::pair<int, std::string>> player_hand, std::pair<int, std::string>  dealer_hand, std::string  choice, const int& count)
            : player_hand_(std::move(player_hand)), dealer_hand_(std::move(dealer_hand)), choice_(std::move(choice)), count_(count)
    {
    }

    void run();

private:
    const std::pair<std::pair<int, std::string>, std::pair<int, std::string>> player_hand_;
    const std::pair<int, std::string> dealer_hand_;
    const std::string choice_;
    const int count_;

};

class SimTaskNonPar : public Task
{
public:
    SimTaskNonPar(const int& DA, std::pair<std::pair<int, std::string>, std::pair<int, std::string>> player_hand, std::pair<int, std::string>  dealer_hand, std::string  choice)
            : deckAmount_(DA),player_hand_(std::move(player_hand)), dealer_hand_(std::move(dealer_hand)), choice_(std::move(choice))
    {
    }

    void run();

private:
    const int deckAmount_;
    const std::pair<std::pair<int, std::string>, std::pair<int, std::string>> player_hand_;
    const std::pair<int, std::string> dealer_hand_;
    const std::string choice_;

};

class StartTask : public Task
{

public:
    StartTask()
    {
    }

    void run();

private:

};

#endif
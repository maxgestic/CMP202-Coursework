//
// Created by maxri on 10/05/2021.
//

#ifndef PROJECT_GLOBAL_H
#define PROJECT_GLOBAL_H
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>

extern std::vector<std::vector<int>> results;
extern std::mutex result_mutex;

extern std::condition_variable ready_cv;
extern std::mutex ready_mutex;
extern bool ready;
extern int deckAmount;

extern std::chrono::steady_clock::time_point start;

class global {


};


#endif //PROJECT_GLOBAL_H

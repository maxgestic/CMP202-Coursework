#include "farm.h"
#include <thread>
#include <iostream>

void Farm::add_task(Task *task)
{

    queue_lock.lock();

    queue_of_tasks.push(task);

    queue_lock.unlock();

}

int Farm::size(){

    return queue_of_tasks.size();

}

void Farm::run()
{
    // FIXME - Implement this

    auto cores = std::thread::hardware_concurrency();

    std::vector<std::thread> threads;
    threads.reserve(cores);

    int temp = 0;

    for (int i = 0; i < cores; i++){

        threads.emplace_back(std::thread(

                [&]{

                    while (!queue_of_tasks.empty()){

                        queue_lock.lock();

                        Task* task = queue_of_tasks.front();

//                        if (temp % 1000 == 0) std::cout << temp << "\n";
//
//                        temp++;


                        queue_of_tasks.pop();

                        queue_lock.unlock();

                        task->run();

                        delete task;

                    }

                }));



    }
    for (int i = 0; i < cores; i++){

        threads[i].join();

    }

}

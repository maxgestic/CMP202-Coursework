#ifndef FARM_H
#define FARM_H

#include "task.h"
#include <queue>
#include <mutex>

/** A collection of tasks that should be performed in parallel. */
class Farm {
public:
    // DO NOT CHANGE the public interface of this class.
    // You only need to implement the existing methods.

    /** Add a task to the farm.
        The task will be deleted once it has been run. */
    void add_task(Task *task);

    /** Run all the tasks in the farm.
        This method only returns once all the tasks in the farm
        have been completed. */
    void run();


    int size();

private:

    std::queue<Task *> queue_of_tasks;

    std::mutex queue_lock;
};

#endif

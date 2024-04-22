#include "Task_Queue.hpp"

#include <condition_variable> 
#include <string>
#include <mutex> 
#include <queue> 
#include <vector>
#include <regex>

using namespace std;
using namespace Queue;

Task_Queue::Task_Queue()
{
    task_queue_ = {};
};

void Task_Queue::put(Task task)
{
    {
        unique_lock<mutex> lock(queue_mutex_);
        task_queue_.emplace(move(task));
    }
    cv_.notify_one();
};

Task_Queue::Task Task_Queue::get()
{
    Task task;

    {
        unique_lock<mutex> lock(queue_mutex_);

        cv_.wait(lock, [this]{
            return !task_queue_.empty();
        });

        task = move(task_queue_.front());

        task_queue_.pop();
    }

    cv_.notify_one();

    return task;
};
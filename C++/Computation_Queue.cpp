#include "Computation_Queue.hpp"

#include <condition_variable> 
#include <string>
#include <mutex> 
#include <queue> 
#include <vector>
#include <regex>

using namespace std;
using namespace Computation;

Computation_Queue::Computation_Queue()
{
    task_queue_ = {};
};

void Computation_Queue::put(Comp_Unit unit)
{
    {
        unique_lock<mutex> lock(queue_mutex_);
        task_queue_.emplace(move(unit));
    }
    cv_.notify_one();
};

Computation_Queue::Comp_Unit Computation_Queue::get()
{
    Comp_Unit unit;

    {
        unique_lock<mutex> lock(queue_mutex_);

        cv_.wait(lock, [this]{
            return !task_queue_.empty();
        });
        
        unit = move(task_queue_.front());
        task_queue_.pop();
    }
    cv_.notify_one();

    return unit;
};
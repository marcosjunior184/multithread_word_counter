#ifndef _TASK_QUEUE_H_
#define _TASK_QUEUE_H_

#include <condition_variable>
#include <iostream>
#include <string>
#include <vector>
#include <mutex> 
#include <queue> 

namespace Queue
{
    class Task_Queue
    {

        public:
            struct Task{
                std::vector<std::string> _task;
                bool is_sentinel;
            };

            Task_Queue();
            void put(Task task);
            Task get();
            bool empty();

        private:
            std::queue<Task> task_queue_;
            std::mutex queue_mutex_;
            std::condition_variable cv_;  

    };
}

#include "Task_Queue.cpp"

#endif